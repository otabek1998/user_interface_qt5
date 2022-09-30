#include "include/homescreen.h"
#include "include/touchinput.h"
#include <stdio.h>
#include <QApplication>

static void touchButtonDown(int x, int y, void* arg)
{
    printf("Got up event on (%d,%d)\n", x, y);
    const std::map<int, String> buttonCoordinates =
    {
        {1300, HOME_BTN},
        {1400, VOICEDIAL_BTN},
        {1500, POWER_BTN},
        {2000, VOLUP_BTN},
        {2100, MUTE_BTN},
        {2200, VOLDOWN_BTN},
        {2300, MUSIC_BTN},
        {2400, PHONE_BTN}
    };
    auto search = buttonCoordinates.find(x);
    if(search == buttonCoordinates.end()){
        return;
    }
    cout << search->second << endl;


}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeScreen w;
    w.show();

    static Offboard::EventsCallbacks keyClbks = {
         &touchButtonDown,
         nullptr,
         nullptr,
         nullptr,
         nullptr
     };
    Offboard::TouchInput ti(keyClbks);
    a.exec();
    ti.runinThread();

    return 0;
}
