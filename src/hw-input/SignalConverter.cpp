#include "SignalConverter.hpp"
#include "SignalTraits.hpp"

#include <cstdint>
#include <map>
#include <unordered_map>
#include <cstring>
#include <atomic>
#include <dbus/dbus.h>
#include <dlt.h>
#include <dlt_cpp_extension.hpp>

using namespace HWInput;

DLT_IMPORT_CONTEXT(HWInputDltContext)

//------------------------------------------------------------------------------
struct SignalConverter::Impl
{
    using SignalMap = std::unordered_map<std::string, SignalTraits>;
    using ButtonStateMap = std::map<Button, bool>;

    ICallback* mCallback {nullptr};
    DBusConnection* mConnection {nullptr};
    SignalMap mSignals;
    ButtonStateMap mButtonState;
    std::string mPath {"/com/clarion/ivi/InputHandler"};
    std::string mInterface {"com.clarion.ivi.InputHandler"};
    std::string mWakeUpSignal {"WakeUp"};
    std::atomic_bool m_pauseProcessing {false};

    //--------------------------------------------------------------------------
    static constexpr const uint8_t SF_PRESS = 0x01;
    static constexpr const uint8_t SF_RELEASE = 0x02;
    static constexpr const uint8_t SF_IDLE = 0x04;

    //--------------------------------------------------------------------------
    explicit Impl(ICallback* pCallback):
        mCallback(pCallback)
    {
        using ConfigPair = std::pair<std::string, Button>;
        static const ConfigPair config[] =
        {
            {"HOME",        Button::BUTTON_HOME},
            {"MUTE",        Button::BUTTON_MUTE},
            {"POWER",       Button::BUTTON_POWER},
            {"VOLUP",       Button::BUTTON_VOLUP},
            {"VOLDN",       Button::BUTTON_VOLDN},
            {"VOICEDIAL",   Button::BUTTON_VOICEDIALSW},
            {"SETTING",     Button::BUTTON_SETTING},
            {"SEEKUP",      Button::BUTTON_SEEKUP},
            {"SEEKDN",      Button::BUTTON_SEEKDN},
            {"TELON",       Button::BUTTON_TELON},
            {"TELOFF",      Button::BUTTON_TELOFF},
            {"SOURCE",      Button::BUTTON_SOURCE},
            {"FLASH",       Button::BUTTON_FLASH},
            {"MUSIC",       Button::BUTTON_MUSIC},
            {"PHONE",       Button::BUTTON_PHONE}
        };

        for (const ConfigPair& pair: config)
        {
            SignalTraits traits(pair.first, pair.second);
            mSignals[traits.signal] = traits;
            mButtonState[pair.second] = false;
        }

        // If this function is not called, the D-Bus library will not lock any data structures.
        // If it is called, D-Bus will do locking, at some cost in efficiency.
        // Since D-Bus 1.7 it is safe to call this function from any thread,
        // any number of times (but it must be called before any other libdbus API is used).
        if (!dbus_threads_init_default())
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, "D-Bus threads initialization is failed");
        }
    }

    ~Impl()
    {
        if (mConnection)
        {
            dbus_connection_close(mConnection);
            dbus_connection_unref(mConnection);
            mConnection = nullptr;
        }
    }

    //--------------------------------------------------------------------------
    /**
     * @brief Process an incoming D-Bus message
     * @param message Incoming D-Bus message
     */
    static DBusHandlerResult processMessage(DBusConnection* /*connection*/, DBusMessage* message, void* user_data)
    {
        Impl* pImpl = static_cast<Impl*>(user_data);

        int messageType = dbus_message_get_type(message);
        const char* messageName = dbus_message_get_member(message);

        DLT_LOG(
            HWInputDltContext,
            DLT_LOG_DEBUG,
            DLT_CSTRING("Processing message: "),
            DLT_CSTRING(messageName));

        if (!pImpl->m_pauseProcessing)
        {
            if ((messageType == DBUS_MESSAGE_TYPE_SIGNAL) &&
                pImpl->mSignals.find(messageName) != pImpl->mSignals.end())
            {
                pImpl->processSignal(message);
            }
        }

        return DBUS_HANDLER_RESULT_HANDLED;
    }

    //--------------------------------------------------------------------------
    /**
     * @brief Retrieves button event flags from D-Bus message
     * @param message PP IPC S_<BUTTON>_BTN D-Bus message
     * @return Message payload (event flags, see Power Processor IPC docs)
     */
    static uint8_t getSignalArgument(DBusMessage* message)
    {
        uint8_t* dataPtr = nullptr;
        uint32_t len = 0;
        if (!dbus_message_get_args(
                message,
                nullptr,
                DBUS_TYPE_ARRAY,
                DBUS_TYPE_BYTE,
                &dataPtr,
                &len,
                DBUS_TYPE_INVALID))
        {
            const char* signature = dbus_message_get_signature(message);
            DLT_LOG(
                HWInputDltContext,
                DLT_LOG_ERROR,
                DLT_CSTRING("Invalid message signature: expected signature 'b', got "),
                DLT_CSTRING(signature));
        }

        if (!dataPtr || len == 0) {
            DLT_LOG(
                HWInputDltContext,
                DLT_LOG_ERROR,
                DLT_CSTRING("Failed to retrieve D-Bus message payload"));
            return 0;
        }

        return *dataPtr;
    }

    //--------------------------------------------------------------------------
    void sendButtonUpdate(Button control, bool state)
    {
        if (state)
        {
            mCallback->onButtonDown(control, HWInput::ButtonType::STEERING_SW);
        }
        else
        {
            mCallback->onButtonUp(control, HWInput::ButtonType::STEERING_SW);
        }
    }

    //--------------------------------------------------------------------------
    void updateButton(Button control, bool newState)
    {
        auto iter = mButtonState.find(control);
        if (iter == mButtonState.end() || iter->second != newState)
        {
            mButtonState[control] = newState;
            sendButtonUpdate(control, newState);
        }
    }

    //--------------------------------------------------------------------------
    /**
     * @brief Convert D-Bus signal from PP IPC into a callback invocation
     * @param message PP IPC S_<BUTTON>_BTN D-Bus message
     */
    void processSignal(DBusMessage* message)
    {
        if (!mCallback)
        {
            DLT_LOG(
                HWInputDltContext,
                DLT_LOG_ERROR,
                DLT_STRING("No callback is specified for HW Input Handler"));
            return;
        }

        const std::string messageName = dbus_message_get_member(message);
        const SignalTraits& traits = mSignals[messageName];

        uint8_t flags = getSignalArgument(message);

        if ((flags & (SF_PRESS | SF_RELEASE | SF_IDLE)) != 0)
        {
            updateButton(traits.control, (flags & SF_PRESS) != 0);
        }

        DLT_LOG(
            HWInputDltContext,
            DLT_LOG_DEBUG,
            DLT_CSTRING("Got message: "),
            DLT_CSTRING(traits.signal.c_str()),
            DLT_CSTRING(", "),
            DLT_HEX8(flags),
            DLT_CSTRING(", control is "),
            DLT_HEX8(static_cast<uint8_t>(traits.control)));

        // Send reply
        DBusMessage* reply = dbus_message_new_signal(
                mPath.c_str(),
                mInterface.c_str(),
                traits.reply.c_str());

        const uint8_t* flagPtr = &flags;
        if (!reply)
        {
            DLT_LOG(
                HWInputDltContext,
                DLT_LOG_ERROR,
                DLT_CSTRING("Failed to create a D-Bus reply signal"));
        }

        if (!dbus_message_append_args(
                reply,
                DBUS_TYPE_ARRAY,
                DBUS_TYPE_BYTE,
                &flagPtr,
                1,
                DBUS_TYPE_INVALID))
        {
            DLT_LOG(
                HWInputDltContext,
                DLT_LOG_ERROR,
                DLT_CSTRING("Failed to add arguments to D-Bus reply signal"));
            dbus_message_unref(reply);
            return;
        }

        uint32_t clientSerial = 0;
        if (!dbus_connection_send(mConnection, reply, &clientSerial))
        {
            DLT_LOG(
                HWInputDltContext,
                DLT_LOG_ERROR,
                DLT_CSTRING("Failed to send a reply signal"));
        }

        dbus_message_unref(reply);
    }

    //--------------------------------------------------------------------------
    /**
     * Sends a signal intercepted by the Input Handler's loop to wake up from IO
     */
    void wakeUp()
    {
        if (mConnection)
        {
            dbus_connection_close(mConnection);
        }
    }

    //--------------------------------------------------------------------------
    void process()
    {
        while (dbus_connection_get_is_connected(mConnection))
        {
            dbus_connection_read_write_dispatch(mConnection, 1000);
        }
        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_INFO, "D-Bus connection is closed");
    }

    //--------------------------------------------------------------------------
    void pauseProcessing()
    {
        m_pauseProcessing = true;
    }

    //--------------------------------------------------------------------------
    void resumeProcessing()
    {
        m_pauseProcessing = false;
    }

    //--------------------------------------------------------------------------
    bool connect()
    {
        mConnection = dbus_bus_get_private(DBUS_BUS_SYSTEM, nullptr);
        if (!mConnection)
        {
            return false;
        }

        const size_t bufferSize = 512;
        char matchRule[bufferSize];
        for (const auto& signal : mSignals)
        {
            const auto& signalName = signal.second.signal;
            snprintf(matchRule, bufferSize, "type='signal',member='%s'", signalName.c_str());
            dbus_bus_add_match(mConnection, matchRule, nullptr);
        }
        snprintf(matchRule,
                bufferSize,
                "type='signal',interface='%s',member='%s'",
                mInterface.c_str(),
                mWakeUpSignal.c_str());

        dbus_bus_add_match(mConnection, matchRule, nullptr);

        if (!dbus_connection_add_filter(mConnection, &Impl::processMessage, this, nullptr))
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, "Failed to add message filter");
            dbus_connection_close(mConnection);
            dbus_connection_unref(mConnection);
            return false;
        }

        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_INFO, "D-Bus connection created");

        return true;
    }
};

//------------------------------------------------------------------------------
SignalConverter::~SignalConverter()
{
}

//------------------------------------------------------------------------------
SignalConverter::SignalConverter(ICallback *pCallback)
{
    mImpl.reset(new Impl(pCallback));
}

//------------------------------------------------------------------------------
bool SignalConverter::connect()
{
    return mImpl->connect();
}

//------------------------------------------------------------------------------
void SignalConverter::process()
{
    mImpl->process();
}

//------------------------------------------------------------------------------
void SignalConverter::wakeUp()
{
    mImpl->wakeUp();
}

//------------------------------------------------------------------------------
void SignalConverter::pauseProcessing()
{
    mImpl->pauseProcessing();
}

//------------------------------------------------------------------------------
void SignalConverter::resumeProcessing()
{
    mImpl->resumeProcessing();
}
