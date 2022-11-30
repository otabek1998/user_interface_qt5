#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QThread>
#include <string>
#include <iostream>
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
    std::string getLeftFrameContent();
    std::string getRightFrameContent();
    std::string getMainFrameContent();

    void setLeftFrameContent(std::string);
    void setRightFrameContent(std::string);
    void setMainFrameContent(std::string);

    std::string frame_array[3];
    void setAllFrames();
    void resetAllFrames();

    bool visible;
    Ui::HomeScreen *ui;
    void Initialize();
    void displayDayOfWeek();
    void displayDate(int side); // side 0 is right frame, 1 for right, 2 for left
    void clockAsMainFrame(QTimer *timer);
    void clockAsSideFrame(QTimer *timer, int side); // side 1 for right, 2 for left
    void vehicleDataAsMainFrame();
    void vehicleDataAsSideFrame(int side);
    void playerAsMainFrame();
    void playerAsSideFrame(int side);
    void onPowerButtonPress();
    void onFrameHoldGesture();
    void onFrameChangerCancelPress();

    QTimer *timer;
    QWidget *analogTime;
    QLabel *digitTime;
    QLabel *dayOfWeek;
    QLabel *Date;
    QLabel *songName;
    QLabel *artistName;
    QPushButton *play;
    QPushButton *previous;
    QPushButton *next;
    QWidget *sourceWidgetOnSideFrame;
    QLabel *songNameOnSideFrame;
    QPushButton *playOnSideFrame;
private slots:
    void showTimeOnStatusBar();
    void showTimeOnMainFrame();
    void setArtistOnMainFrame();
    void setSongNameOnMainFrame();
    void setAlbumArtOnMainFrame();
    void setSongNameOnSideFrame();
};

#endif // HOMESCREEN_H
