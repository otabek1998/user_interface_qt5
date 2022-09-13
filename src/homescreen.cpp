#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "include/homescreen.h"
#include "ui_homescreen.h"

HomeScreen::HomeScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeScreen)
{
    ui->setupUi(this);
    this->Initialize();
}

HomeScreen::~HomeScreen()
{
    delete ui;
}

void HomeScreen::Initialize()
{
    QString widgetStyle = "QWidget#MainMenu {"
                          "background-image: url(:/home_screen/background/Bitmaps/home_screen/0569-bg_1_home.png);"
                          "background-repeat: no-repeat}";
    QTimer *timer = new QTimer(this);

    this->setObjectName("MainMenu");
    this->setStyleSheet(widgetStyle);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
}

void HomeScreen::showTime()
{
    QTime time = QTime::currentTime();
    int hour, hour2, minute, minute2 ;

    hour = time.hour();
    minute = time.minute();
    minute2 = minute%10;
    hour2 = hour%10;
    hour /= 10;
    minute /= 10;

    ui->hour1_clock->setStyleSheet(digitImageStringify(hour));
    ui->hour2_clock->setStyleSheet(digitImageStringify(hour2));
    ui->minute1_clock->setStyleSheet(digitImageStringify(minute));
    ui->minute2_clock->setStyleSheet(digitImageStringify(minute2));
}

QString HomeScreen::digitImageStringify(int label)
{
    QString stylesheetformat, int_to_str;

    int_to_str = QString::number(label);
    stylesheetformat = "border-image: url(:/home_screen/background/Bitmaps/home_screen/clock_font24/hours/" + int_to_str + ".png) 0 0 0 0 stretch stretch;";
    return stylesheetformat;
}
