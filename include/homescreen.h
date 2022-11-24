#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QThread>
#include "include/screensaver.h"
#include "include/touchinput.h"
#include "include/framechanger.h"
#include "include/musicplayer.h"

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
    MusicPlayer *player;
    TouchInput *ti;
    FrameChanger *fc;
    QThread *ti_thread;
    QThread *music_thread;
private:
    bool visible;
    Ui::HomeScreen *ui;
    void Initialize();
    void displayDayOfWeek();
    void displayDate(int side); // side 0 is main frame, 1 for right, 2 for left
    void clockAsMainFrame(QTimer *timer);
    void clockAsSideFrame(QTimer *timer, int side); // side 1 for right, 2 for left
    void vehicleDataAsMainFrame();
    void vehicleDataAsSideFrame(int side);
    void playerAsMainFrame();
    void playerAsSideFrame();
    void onPowerButtonPress();
    void onFrameHoldGesture();
    void onFrameChangerCancelPress();

    QWidget *analogTime;
    QLabel *digitTime;
    QLabel *dayOfWeek;
    QLabel *Date;
    QLabel *songName;
    QLabel *artistName;
    QPushButton *play;
    QPushButton *previous;
    QPushButton *next;
private slots:
    void showTimeOnStatusBar();
    void showTimeOnMainFrame();
    void setArtistOnMainFrame();
    void setSongNameOnMainFrame();
};

#endif // HOMESCREEN_H
