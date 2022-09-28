#include "SoftkeyConverter.hpp"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <libinput.h>
#include <libudev.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <dlt.h>
#include <dlt_cpp_extension.hpp>

#include "HWInput/ICallback.hpp"

DLT_IMPORT_CONTEXT(HWInputDltContext)

static const libinput_interface LibInputFileInterface =
{
	/* .open_restricted =  */[](const char* path, int flags, void* /* user_data */)
    {
        int fd = open(path, flags);
        if (fd < 0)
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, "Failed to open", path, "with error:", strerror(errno));
        }

        return fd < 0 ? -errno : fd;
    },

	/* .close_restricted =  */[](int fd, void* /* user_data */)
    {
        close(fd);
    }
};

namespace HWInput
{
    const std::map<std::pair<long, long>, Button> SoftkeyConverter::CoordinatesToButtonMap
    {
        { { 1300, 1300 }, Button::BUTTON_HOME      },
        { { 1400, 1400 }, Button::BUTTON_VOICEDIAL },
        { { 1500, 1500 }, Button::BUTTON_POWER     },
        { { 2000, 2000 }, Button::BUTTON_VOLUP     },
        { { 2100, 2100 }, Button::BUTTON_MUTE      },
        { { 2200, 2200 }, Button::BUTTON_VOLDN     },
        { { 2300, 2300 }, Button::BUTTON_MUSIC     },
        { { 2400, 2400 }, Button::BUTTON_PHONE     }
    };

    SoftkeyConverter::SoftkeyConverter(ICallback* callback)
        : m_pCallback{callback}
    {
    }

    SoftkeyConverter::~SoftkeyConverter()
    {
        if (m_pInputCtx)
        {
            libinput_unref(m_pInputCtx);
        }
    }

    bool SoftkeyConverter::connect()
    {
        DLT_LOG_FCN_CXX(HWInputDltContext, DLT_LOG_DEBUG, "");

        udev *udevInst = udev_new();
        if (!udevInst)
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, "Failed to initialize udev");
            return false;
        }

        m_pInputCtx = libinput_udev_create_context(&LibInputFileInterface, nullptr, udevInst);
        udev_unref(udevInst);

        if (!m_pInputCtx)
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, "Failed to initialize context from udev");
            return false;
        }

        if (libinput_udev_assign_seat(m_pInputCtx, "seat0") != 0)
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, "Failed to set seat");

            libinput_unref(m_pInputCtx);
            m_pInputCtx = nullptr;

            return false;
        }

        return true;
    }

    void SoftkeyConverter::process()
    {
        DLT_LOG_FCN_CXX(HWInputDltContext, DLT_LOG_DEBUG, "");

        m_running = true;

        pollfd fds{};
        fds.fd = libinput_get_fd(m_pInputCtx);
        fds.events = POLLIN;

        // handle already-pending device added events
        processEvents();

        while (m_running)
        {
            int ret = poll(&fds, 1, 100);
            if (ret == 0) // timeout
            {
                continue;
            }
            else if (ret < 0 && ret != EINTR)
            {
                DLT_LOG_CXX(HWInputDltContext, DLT_LOG_ERROR, "poll call failed:", strerror(errno));
                return;
            }

            processEvents();
        }
    }

    void SoftkeyConverter::wakeUp()
    {
        DLT_LOG_FCN_CXX(HWInputDltContext, DLT_LOG_DEBUG, "");

        m_running = false;
    }

    void SoftkeyConverter::processEvents()
    {
        libinput_dispatch(m_pInputCtx);
        libinput_event* ev{nullptr};

        while ((ev = libinput_get_event(m_pInputCtx)) != nullptr)
        {
            auto evType = libinput_event_get_type(ev);

            // event need to be destoyed, otherwise it will stuck in the queue
            if (!m_pauseProcessing)
            {
                switch (evType)
                {
                case LIBINPUT_EVENT_DEVICE_ADDED:
                    // only for debug purposes
                    DLT_LOG_CXX(HWInputDltContext, DLT_LOG_DEBUG, "got device added event. device name:",
                        libinput_device_get_name(libinput_event_get_device(ev)));
                    break;

                case LIBINPUT_EVENT_TOUCH_DOWN:
                    processTouchDownEvent(ev);
                    break;

                case LIBINPUT_EVENT_TOUCH_UP:
                case LIBINPUT_EVENT_TOUCH_CANCEL:
                    processTouchUpEvent(ev);
                    break;

                default:
                    // we are not interested in other events
                    break;
                }
            }

            libinput_event_destroy(ev);
            libinput_dispatch(m_pInputCtx);
        }
    }

    void SoftkeyConverter::processTouchDownEvent(libinput_event* ev)
    {
        libinput_event_touch* t = libinput_event_get_touch_event(ev);
        double x = libinput_event_touch_get_x(t);
        double y = libinput_event_touch_get_y(t);
        int32_t slot = libinput_event_touch_get_slot(t);

        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_DEBUG, "touch DOWN event: x=", x, "y=", y, "slot=", slot);

        auto button = cooordinatesToButton(x, y);
        if (!button.first)
        {
            // not a soft key, nothing to do
            return;
        }

        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_DEBUG, "coordinates map to button:", static_cast<int>(button.second));

        auto res = m_slotToButton.insert(std::make_pair(slot, button.second));
        if (!res.second)
        {
            DLT_LOG_CXX(HWInputDltContext, DLT_LOG_WARN,
                "slot", slot, "already in pressed key map and contains value", static_cast<int>(m_slotToButton[slot]));
            return;
        }

        m_pCallback->onButtonDown(button.second, HWInput::ButtonType::SOFTKEY);
    }

    void SoftkeyConverter::processTouchUpEvent(libinput_event* ev)
    {
        libinput_event_touch* t = libinput_event_get_touch_event(ev);
        int32_t slot = libinput_event_touch_get_slot(t);

        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_DEBUG, "touch UP event: slot=", slot);

        auto it = m_slotToButton.find(slot);
        if (it == m_slotToButton.end())
        {
            // not a soft key, ignore it
            return;
        }

        auto button = it->second;
        m_slotToButton.erase(it);

        DLT_LOG_CXX(HWInputDltContext, DLT_LOG_DEBUG, "release button", static_cast<int>(button));
        m_pCallback->onButtonUp(button, HWInput::ButtonType::SOFTKEY);
    }

    std::pair<bool, Button> SoftkeyConverter::cooordinatesToButton(double x, double y) const
    {
        auto xy = std::make_pair(lround(x), lround(y));
        auto it = CoordinatesToButtonMap.find(xy);

        if (it != CoordinatesToButtonMap.end())
        {
            return std::make_pair(true, it->second);
        }
        return std::make_pair(false, Button::BUTTON_HOME);
    }
}
