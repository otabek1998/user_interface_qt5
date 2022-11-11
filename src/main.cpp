#include "include/homescreen.h"
#include "include/touchinput.h"
#include "include/screensaver.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <QApplication>
#include <gst/gst.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeScreen w;
    gst_init(&argc, &argv);
    w.showFullScreen();
    w.ti_thread->start();
    a.exec();

    return 0;
}
