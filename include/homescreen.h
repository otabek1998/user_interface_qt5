#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QThread>
#include "include/screensaver.h"
#include "include/touchinput.h"
#include "include/framechanger.h"

namespace Ui {
class HomeScreen;
}

class HomeScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = 0);
    ~HomeScreen();
    void displayClockAsMainFrame();
    QString digitImageStringify(int);
    ScreenSaver *screenSaver;
    TouchInput *ti;
    FrameChanger *fc;
    QThread *ti_thread;
private:
    bool visible;
    Ui::HomeScreen *ui;
    void Initialize();
    void displayDayOfWeek();
    void displayDate(int side); // side 0 is main frame, 1 for right, 2 for left
    void clockAsMainFrame(QTimer *timer);
    void clockAsSideFrame(QTimer *timer, int side); // side 1 for right, 2 for left
    void onPowerButtonPress();
    void onFrameHoldGesture();
    void onFrameChangerCancelPress();

    QWidget *analogTime;
    QLabel *digitTime;
    QLabel *dayOfWeek;
    QLabel *Date;
private slots:
    void showTimeOnStatusBar();
    void showTimeOnMainFrame();
};

#endif // HOMESCREEN_H
