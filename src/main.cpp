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
    HomeScreen w;
    w.showFullScreen();
    w.ti_thread->start();
    //w.music_thread->start();
    a.exec();

    return 0;
}
