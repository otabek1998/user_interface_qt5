#include "include/homescreen.h"
#include "include/touchinput.h"
#include "include/screensaver.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <QApplication>

static void touchButtonDown(int x, int y, void* arg)
{
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
    auto search = buttonCoordinates.find(x);
    if(search == buttonCoordinates.end()){
        return;
    }
    std::cout << search->second << std::endl;
    if (search->second == "POWER_BTN"){
        ScreenSaver *s = new ScreenSaver;
    }


}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeScreen w;
    w.showFullScreen();

    static Offboard::EventsCallbacks keyClbks = {
         &touchButtonDown,
         nullptr,
         nullptr,
         nullptr,
         nullptr
     };
    Offboard::TouchInput ti(keyClbks);
    ti.runinThread();
    a.exec();

    return 0;
}
