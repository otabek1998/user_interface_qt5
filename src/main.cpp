#include "include/homescreen.h"
#include "include/touchinput.h"
#include "include/screensaver.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    static Offboard::EventsCallbacks keyClbks = {
         nullptr,
         nullptr,
         nullptr,
         nullptr,
         nullptr
     };
    Offboard::TouchInput *ti = new Offboard::TouchInput(keyClbks);
    HomeScreen w(ti, 0);
    ti->runinThread();
    w.show();
    a.exec();

    return 0;
}
