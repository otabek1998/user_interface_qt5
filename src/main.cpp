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
<<<<<<< HEAD
    HomeScreen w;
    w.show();
    w.ti_thread->start();
=======
    static Offboard::EventsCallbacks keyClbks = {
         nullptr,
         nullptr,
         nullptr,
         nullptr,
         nullptr
     };
    Offboard::TouchInput ti(keyClbks);
    HomeScreen w(&ti, 0);
    //ti->runinThread();
    w.show();
    ti.runinThread();
>>>>>>> 0dd4bd9b4ca4114f79521ae14bc5a9b60de784fc
    a.exec();

    return 0;
}
