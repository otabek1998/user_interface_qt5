#ifndef TOUCHINPUT_H
#define TOUCHINPUT_H

#include <pthread.h>
#include <libinput.h>
#include <QObject>
#include <map>

class TouchInput : public QObject {
    Q_OBJECT
public:
    TouchInput();
    ~TouchInput();
    void Process();
    void runinThread();
signals:
    void emitPowerButtonSignal();
    void emitVolumeUpSignal();
    void emitVolumeDownSignal();
private:
    void processTouchUpEvent(libinput_event* ev);
    void processTouchDownEvent(libinput_event* ev);
    void internalProcess();
    static void* thread_func(void *arg);
    struct libinput* inputCtx;
    struct libinput_device* touchDev;
    pthread_t threadid;
    bool m_running;
};

#endif // TOUCHINPUT_H
