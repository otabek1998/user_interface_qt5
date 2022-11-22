#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "include/touchinput.h"
#include <stdio.h>
#include <poll.h>
#include <iostream>

static const char* touchDevice = "/dev/input/touchscreen0";

const std::map<int, std::string> buttonCoordinates =
{
    {1300, "HOME_BTN"},
    {1400, "VOICEDIAL_BTN"},
    {1500, "POWER_BTN"},
    {2000, "VOLUP_BTN"},
    {2100, "MUTE_BTN"},
    {2200, "VOLDOWN_BTN"},
    {2300, "MUSIC_BTN"},
    {2400, "PHONE_BTN"}
};

static const libinput_interface LibInputFileInterface = {
    /* .open_restricted =  */[](const char* path, int flags, void* /* user_data */) -> int
    {
        int fd = open(path, flags);
        return fd < 0 ? -errno : fd;
    },

    /* .close_restricted =  */[](int fd, void* /* user_data */)
    {
        close(fd);
    }
};

TouchInput::TouchInput() : threadid(0)
{
    inputCtx = libinput_path_create_context(&LibInputFileInterface, nullptr);
    touchDev = libinput_path_add_device(inputCtx, touchDevice);
}

TouchInput::~TouchInput()
{
    if (threadid)
        pthread_cancel(threadid);
    libinput_path_remove_device(touchDev);
    libinput_unref(inputCtx);
}

void TouchInput::internalProcess()
{
    libinput_dispatch(inputCtx);
    libinput_event* ev{nullptr};

    while ((ev = libinput_get_event(inputCtx)) != nullptr)
    {
        auto evType = libinput_event_get_type(ev);

        switch (evType)
        {

        case LIBINPUT_EVENT_DEVICE_ADDED:
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

        libinput_event_destroy(ev);
        libinput_dispatch(inputCtx);
    }
}

void TouchInput::runinThread()
{
    pthread_create(&threadid, nullptr, &TouchInput::thread_func, this);
}

void TouchInput::processTouchDownEvent(libinput_event *ev)
{
    libinput_event_touch* t = libinput_event_get_touch_event(ev);
    double x = libinput_event_touch_get_x(t);
    double y = libinput_event_touch_get_y(t);
    int xi = static_cast<int>(x);
    int yi = static_cast<int>(y);
    auto search = buttonCoordinates.find(x);
    if(search == buttonCoordinates.end()){
        return;
    }
    std::cout << search->second << std::endl;

    if (search->first == 1500){
        emitPowerButtonSignal();
    }

    if (search->first == 2000){
        emitVolumeUpSignal();
    }

    if(search->first == 2200){
        emitVolumeDownSignal();
    }
}

void TouchInput::processTouchUpEvent(libinput_event *ev)
{
    libinput_event_touch* t = libinput_event_get_touch_event(ev);
}

void TouchInput::Process()
{
    m_running = true;

    pollfd fds{};
    fds.fd = libinput_get_fd(inputCtx);
    fds.events = POLLIN;

    // handle already-pending device added events
    internalProcess();

    while (m_running)
    {
        int ret = poll(&fds, 1, 100);
        if (ret == 0) // timeout
        {
            continue;
        }
        else if (ret < 0 && ret != EINTR)
        {
            return;
        }

        internalProcess();
    }
}

void* TouchInput::thread_func(void *arg)
{
    TouchInput *that = static_cast<TouchInput*>(arg);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    that->Process();
}

