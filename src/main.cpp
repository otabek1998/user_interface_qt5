#include "include/homescreen.h"
#include "include/touchinput.h"
#include <stdio.h>
#include <QApplication>

static void touchButtonUp(int x, int y, void* arg)
{
    printf(stderr, "Got up event on (%d,%d)\n", x, y);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeScreen w;
    w.show();

    static Offboard::EventsCallbacks keyClbks = {
         &touchButtonUp,
         nullptr,
         nullptr,
         nullptr,
         kt.get()
     };
    Offboard::TouchInput ti(keyClbks);
    ti.Process();

    return a.exec();
}
