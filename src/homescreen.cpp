#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <qpainter.h>
#include "include/homescreen.h"
#include "include/analogclock.h"
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
    QTime currTime = QTime::currentTime();
    AnalogClock *analogClock = new AnalogClock(ui->mainFrameUpperLeftWidget, timer);

    QHBoxLayout *mainUpRightLayout = new QHBoxLayout(ui->mainFrameUppeRightWidget);
    HomeScreen::digitTime = new QLabel(ui->mainFrameUppeRightWidget);
    digitTime->setAlignment(Qt::AlignCenter);
    mainUpRightLayout->addWidget(digitTime);
    digitTime->setStyleSheet("color: white;"
                            "font-size: 30px;");

    QVBoxLayout *mainFrameLowerLayout = new QVBoxLayout(ui->mainFrameLowerWidget);
    HomeScreen::dayOfWeek = new QLabel(ui->mainFrameLowerWidget);
    HomeScreen::Date = new QLabel(ui->mainFrameLowerWidget);
    dayOfWeek->setAlignment(Qt::AlignCenter);
    Date->setAlignment(Qt::AlignCenter);
    mainFrameLowerLayout->addWidget(dayOfWeek);
    mainFrameLowerLayout->addWidget(Date);
    displayDayOfWeek();


    this->setObjectName("MainMenu");
    ui->mainFrame->setObjectName("MainFrame");


    ui->mainFrame->setStyleSheet("QWidget#MainFrame {"
                                 "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_mid.png);"
                                 "background-repeat: no-repeat;}");



    this->setStyleSheet(widgetStyle);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimeOnStatusBar()));
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimeOnMainFrame()));
    timer->start(1000);
}

void HomeScreen::showTimeOnStatusBar()
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

void HomeScreen::showTimeOnMainFrame()
{
    QTime currTime = QTime::currentTime();
    QString time = currTime.toString("hh:mm");
    HomeScreen::digitTime->setText(time);
}

void HomeScreen::displayDayOfWeek()
{
    QDate *qdateDate = new QDate;
    QString output;
    int day = QDate::currentDate().dayOfWeek();

    if (day == 2){
        output = "Tuesday";
    }
    else{
        output = QString::number(day);
    }
    HomeScreen::dayOfWeek->setText(output);
}

QString HomeScreen::digitImageStringify(int label)
{
    QString stylesheetformat, int_to_str;

    int_to_str = QString::number(label);
    stylesheetformat = "border-image: url(:/home_screen/background/Bitmaps/home_screen/clock_font24/hours/" + int_to_str + ".png) 0 0 0 0 stretch stretch;";
    return stylesheetformat;
}
