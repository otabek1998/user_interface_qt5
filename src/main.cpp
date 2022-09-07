#include "include/homescreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeScreen w;
    w.showFullScreen();

    return a.exec();
}
