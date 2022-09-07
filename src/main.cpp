#include "include/homescreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeScreen w;
    QString widgetStyle = "QWidget#MainMenu {background-image: url(:/home_screen/background/Bitmaps/home_screen/0569-bg_1_home.png);}";
    w.setObjectName("MainMenu");
    w.setStyleSheet(widgetStyle);

    w.showFullScreen();

    return a.exec();
}
