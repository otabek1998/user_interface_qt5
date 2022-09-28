#include <unordered_map>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <map>

#include "HWInput/InputHandler.hpp"
#include "HWInput/ICallback.hpp"
#include "SignalConverter.hpp"
#include "SoftkeyConverter.hpp"

#include <dlt_cpp_extension.hpp>

DLT_DECLARE_CONTEXT(HWInputDltContext)

namespace
{
    static std::map<HWInput::Button, bool> ButtonMatrix =
    {
        { HWInput::Button::BUTTON_VOLUP, false },
        { HWInput::Button::BUTTON_VOLDN, false },
        { HWInput::Button::BUTTON_MUTE, false },
        { HWInput::Button::BUTTON_POWER, false },
        { HWInput::Button::BUTTON_SETTING, false },
        { HWInput::Button::BUTTON_VOICEDIAL, false },
        { HWInput::Button::BUTTON_HOME, false },
        { HWInput::Button::BUTTON_SEEKUP, false },
        { HWInput::Button::BUTTON_SEEKDN, false },
        { HWInput::Button::BUTTON_TELON, false },
        { HWInput::Button::BUTTON_TELOFF, false },
        { HWInput::Button::BUTTON_SOURCE, false },
        { HWInput::Button::BUTTON_FLASH, false },
        { HWInput::Button::BUTTON_MUSIC, false },
        { HWInput::Button::BUTTON_PHONE, false }
    };

    bool isButtonPressed(HWInput::Button controlId)
    {
        return ButtonMatrix[controlId];
    }

    static const uint32_t MAX_HOTKEYS_BUTTON_COMBINATION = 2;
}

using namespace HWInput;

//------------------------------------------------------------------------------
class InputHandler::Impl : public ICallback
{
public:
    /**
     * @brief Constructor
     * @param pCallback Pointer to a button event sink
     */
    explicit Impl(ICallback* pCallback);

    /**
     * @brief Start message processing thread
     */
    void start();

    /**
     * @brief Stop message processing thread
     */
    void stop();

    bool registerHotKey(const std::set<Button>& keys, HotKeyId& id);

    void unregisterHotKey(uint32_t handle);

    void pauseProcessing();

    void resumeProcessing();

    ~Impl();

protected:
    virtual void onButtonDown(Button controlId, ButtonType buttonType) override;
    virtual void onButtonUp(Button controlId, ButtonType buttonType) override;
    virtual void onHotKey(HotKeyId id) override {}

private:
    struct HotKey
    {
        HotKeyId id;
        std::set<Button> buttons;
    };

    /**
     * @brief Thread procedure that implements the D-Bus and (if needed) libinput message loop
     */
    template <typename T>
    void run(T& converter);

    bool hotKeyPressed(HotKey& key);

    std::atomic_bool mRunning;
    std::thread mThread;
    std::thread mSoftkeyThread;
    std::mutex mMutex;
    HWInput::SignalConverter mConverter;
    HWInput::SoftkeyConverter mSoftkeyConverter;
    std::vector<HotKey> mHotKeys;
    ICallback* mCallback;
    bool mSoftkeysPresent{false};
};

//------------------------------------------------------------------------------
InputHandler::Impl::Impl(ICallback *pCallback)
: mRunning(false)
, mConverter(this)
, mSoftkeyConverter(this)
, mCallback(pCallback)
{
    mSoftkeysPresent = true;
}

void InputHandler::Impl::onButtonDown(Button controlId,  ButtonType buttonType)
{
    std::lock_guard<std::mutex> lock(mMutex);

    ButtonMatrix[controlId] = true;

    uint32_t totalPressedButtons = std::count_if
                                    (ButtonMatrix.begin(), ButtonMatrix.end(),
                                    [] (const std::pair<HWInput::Button, bool> btn)
                                    {
                                        return btn.second == true;
                                    });

    HotKey key = {};
    if (hotKeyPressed(key) && (totalPressedButtons == MAX_HOTKEYS_BUTTON_COMBINATION))
    {
        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_INFO, "Hotkey pressed:", key.id);
        mCallback->onHotKey(key.id);
    }
    else
    {
        mCallback->onButtonDown(controlId, buttonType);
    }
}

void InputHandler::Impl::onButtonUp(Button controlId, ButtonType buttonType)
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (isButtonPressed(controlId))
    {
        ButtonMatrix[controlId] = false;
        mCallback->onButtonUp(controlId, buttonType);
    }
}

bool InputHandler::Impl::hotKeyPressed(HotKey& handle)
{
    bool found = false;
    for (const HotKey& key : mHotKeys)
    {
        const std::set<Button>& buttons = key.buttons;
        found = true;
        handle = key;
        for (const Button& btn : buttons)
        {
            if (!ButtonMatrix[btn])
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            break;
        }
    }

    return found;
}

bool InputHandler::Impl::registerHotKey(const std::set<Button>& keys, HotKeyId& id)
{
    static HotKeyId nextId = 0;

    if (keys.size() < 2)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(mMutex);

    auto it = std::find_if(mHotKeys.begin(), mHotKeys.end(), [&](const HotKey& key) {
        return key.buttons == keys;
    });

    if (it != mHotKeys.end())
    {
        // keystrokes specified for the hot key have already been registered by another hot key
        return false;
    }

    nextId++;
    id = nextId;
    mHotKeys.push_back({ id, keys });

    return true;
}

void InputHandler::Impl::unregisterHotKey(HotKeyId id)
{
    std::lock_guard<std::mutex> lock(mMutex);
    auto it = std::find_if(mHotKeys.begin(), mHotKeys.end(), [id](const HotKey& key) {
        return key.id == id;
    });

    if (it != mHotKeys.end())
    {
        mHotKeys.erase(it);
    }
}

//------------------------------------------------------------------------------
void InputHandler::Impl::start()
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mRunning)
    {
        mRunning = true;
        mThread = std::move(std::thread(&Impl::run<SignalConverter>, this, std::ref(mConverter)));

        if (mSoftkeysPresent)
        {
            mSoftkeyThread = std::move(std::thread(&Impl::run<SoftkeyConverter>, this, std::ref(mSoftkeyConverter)));
        }
    }
    else
    {
        DLT_LOG_CXX(
            HWInputDltContext,
            DLT_LOG_WARN,
            "Attempting to start for HW Input Handling thread when it's already running");
    }
}

//------------------------------------------------------------------------------
void InputHandler::Impl::stop()
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mRunning)
    {
        mRunning = false;
        mConverter.wakeUp();
        mSoftkeyConverter.wakeUp();

        if (mThread.joinable())
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_WARN, "mThread.join");
            mThread.join();
        }
        if (mSoftkeyThread.joinable())
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_WARN, "mSoftkeyThread.join");
            mSoftkeyThread.join();
        }
    }
    else
    {
        DLT_LOG_CXX(
            HWInputDltContext,
            DLT_LOG_WARN,
            "Attempting top stop HW Input Handling thread when it's already stopped");
    }
}

//------------------------------------------------------------------------------
InputHandler::Impl::~Impl()
{
    try
    {
        stop();
        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_WARN, "stop");
    }
    catch(const std::exception& e)
    {
        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, e.what());
        // nope, no exceptions leaving the dtor
    }
}

void InputHandler::Impl::pauseProcessing()
{
    mConverter.pauseProcessing();
    mSoftkeyConverter.pauseProcessing();
}

void InputHandler::Impl::resumeProcessing()
{
    mConverter.resumeProcessing();
    mSoftkeyConverter.resumeProcessing();
}

//------------------------------------------------------------------------------
template <typename T>
void InputHandler::Impl::run(T& converter)
{
    try
    {
        if (!converter.connect())
        {
            DLT_LOG_FCN_CXX(HWInputDltContext, DLT_LOG_ERROR, "Failed to connect to converter");
            mRunning = false;
            return;
        }
        // This procedure is supposed to run in a thread.
        while (mRunning)
        {
            converter.process();
        }
    }
    catch(...)
    {
        // nope, don't let it escape
        DLT_LOG_FCN_CXX(HWInputDltContext, DLT_LOG_ERROR,
            "HW Input Handler thread has failed: exception");
        mRunning = false;
    }
}

//------------------------------------------------------------------------------
InputHandler::InputHandler()
{
    DLT_REGISTER_CONTEXT(HWInputDltContext, "HWIH", "Hardware Input Handler");
}

//------------------------------------------------------------------------------
InputHandler::~InputHandler()
{
    DLT_UNREGISTER_CONTEXT(HWInputDltContext);
}

//------------------------------------------------------------------------------
bool InputHandler::registerHotKey(const std::set<Button>& keys, HotKeyId& id)
{
    return mImpl->registerHotKey(keys, id);
}

//------------------------------------------------------------------------------
void InputHandler::unregisterHotKey(HotKeyId id)
{
    mImpl->unregisterHotKey(id);
}

//------------------------------------------------------------------------------
bool InputHandler::init(ICallback *pCallback)
{
    try
    {
        DLT_LOG_FCN_CXX(HWInputDltContext, DLT_LOG_INFO, "");
        mImpl.reset(new Impl(pCallback));
        mImpl->start();
    }
    catch(...)
    {
        DLT_LOG_CXX(
            HWInputDltContext,
            DLT_LOG_ERROR,
            "HW Input Handler failed to initialize. Likely bad alloc or thread failure.");
        return false;
    }

    return true;
}

void InputHandler::pauseProcessing()
{
    mImpl->pauseProcessing();
}

void InputHandler::resumeProcessing()
{
    mImpl->resumeProcessing();
}
