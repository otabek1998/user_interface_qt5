#include "include/screensaver.h"
#include "ui_screensaver.h"

ScreenSaver::ScreenSaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSaver)
{
    ui->setupUi(this);
    //this->setObjectName("MainMenu");
    QString widgetStyle = "QWidget#MainMenu {"
                          "background-image: url(:/home_screen/background/Bitmaps/screensavers/0569-bg_1_home.png);"
                          "background-repeat: no-repeat}";
    this->setObjectName("MainFrame");
    this->setStyleSheet(widgetStyle);
}

ScreenSaver::~ScreenSaver()
{
    delete ui;
}
