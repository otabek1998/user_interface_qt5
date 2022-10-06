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
    w.show();
    a.exec();

    return 0;
}
