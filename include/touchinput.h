#ifndef TOUCHINPUT_H
#define TOUCHINPUT_H

#include <libinput.h>
#include <pthread.h>

namespace Offboard {

struct EventsCallbacks {
    void (*down)(int x, int y, void* arg);
    void (*up) (void* arg);
    void (*double_down) (int x, int y, void* arg);
    void (*double_up) (int x, int y, void* arg);
    void *arg;
};

class TouchInput : public QObject{
    Q_OBJECT
public:
    TouchInput(const struct EventsCallbacks& cbs);
    ~TouchInput();
    void Process();
    void runinThread();
signals:
    void processTouchDownEvent(libinput_event* ev);
private:
    void processTouchUpEvent(libinput_event* ev);
    void internalProcess();
    static void* thread_func(void *arg);
    struct libinput* inputCtx;
    struct libinput_device* touchDev;
    struct EventsCallbacks clbks;
    pthread_t threadid;
    bool m_running;
    const std::map<int, std::string> buttonCoordinates;
};
}

#endif // TOUCHINPUT_H
