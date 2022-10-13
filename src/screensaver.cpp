#include "include/screensaver.h"
#include "ui_screensaver.h"
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>

ScreenSaver::ScreenSaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSaver)
{
    ui->setupUi(this);
    //this->setObjectName("MainMenu");
    //QString widgetStyle = "QWidget#MainMenu {"
    //                      "background-image: url(:/home_screen/background/Bitmaps/screensavers/0569-bg_1_home.png);"
    //                      "background-repeat: no-repeat}";
    //this->setObjectName("MainFrame");
    //this->setStyleSheet(widgetStyle);
}

void ScreenSaver::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ScreenSaver::~ScreenSaver()
{
    delete ui;
}
