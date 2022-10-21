#include "include/screensaver.h"
#include "ui_screensaver.h"
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QTimer>

ScreenSaver::ScreenSaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSaver)
{
    ui->setupUi(this);
    this->setObjectName("screenSaver");
    QString widgetStyle = "QWidget#screenSaver {"
                          "background-image: url(:/home_screen/background/Bitmaps/screensavers/0571-bg_1_home.png);"
                          "background-repeat: no-repeat}";
    this->setStyleSheet(widgetStyle);

    ScreenSaver::analogClockWidget = new QWidget(this);
    QTimer *timer = new QTimer(this);

    QVBoxLayout *screenLayout = new QVBoxLayout(this);

    screenLayout->addWidget(analogClockWidget, 0, Qt::AlignCenter);
    screenLayout->setAlignment(Qt::AlignHCenter);
    analogClockWidget->setFixedWidth(170);
    analogClockWidget->setFixedHeight(170);
    AnalogClock *analogClock = new AnalogClock(analogClockWidget, timer, 0);
    analogClockWidget->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ClockPanel/ic_clock_big_base.png);"
                              "background-repeat: no-repeat;");

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
