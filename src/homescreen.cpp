#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <qpainter.h>
#include <iostream>
#include "include/homescreen.h"
#include "include/analogclock.h"
#include "include/touchinput.h"
#include "include/customframe.h"
#include "ui_homescreen.h"

HomeScreen::HomeScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeScreen)
{
    ui->setupUi(this);
    this->Initialize();
    visible = true;
}

HomeScreen::HomeScreen(Offboard::TouchInput *ti, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeScreen)
{

    this->ti = ti;
    ui->setupUi(this);
    this->Initialize();
    visible = true;
}

HomeScreen::~HomeScreen()
{
    delete ui;
}


void HomeScreen::Initialize()
{
    QTimer *timer = new QTimer(this);
    screenSaver = new ScreenSaver();

    QString widgetStyle = "QWidget#MainMenu {"
                          "background-image: url(:/home_screen/background/Bitmaps/home_screen/0569-bg_1_home.png);"
                          "background-repeat: no-repeat}";
    QString sideWidgetStyle = "CustomFrame#SideFrame {"
                              "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_small.png);"
                              "background-repeat: no-repeat}";

    clockAsMainFrame(timer);
    clockAsSideFrame(timer, 1);
    showTimeOnStatusBar();
    showTimeOnMainFrame();
    displayDayOfWeek();
    displayDate(0);
    displayDate(1);


    this->setObjectName("MainMenu");
    ui->mainFrame->setObjectName("MainFrame");
    CustomFrame *frame = this->findChild<CustomFrame*>("MainFrame"); // taking object from ui
    ui->mainFrame->setStyleSheet("CustomFrame#MainFrame {"
                                 "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_mid.png);"
                                 "background-repeat: no-repeat;}");
    /*frame->setStylesheet("CustomFrame#MainFrame {"
                         "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_mid.png);"
                         "background-repeat: no-repeat;}");*/
    ui->right_frame->setObjectName("SideFrame");
    ui->left_frame->setObjectName("SideFrame");

    ui->right_frame->setStyleSheet(sideWidgetStyle);
    ui->left_frame->setStyleSheet(sideWidgetStyle);

    this->setStyleSheet(widgetStyle);

    static Offboard::EventsCallbacks keyClbks = {
         nullptr,
         nullptr,
         nullptr,
         nullptr,
         nullptr
     };

    //ti = new Offboard::TouchInput(keyClbks);
    //ti->runinThread();
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimeOnStatusBar()));
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimeOnMainFrame()));
    connect(ti, &Offboard::TouchInput::emitPowerButtonSignal, this, &HomeScreen::onPowerButtonPress);


    connect(frame, &CustomFrame::pressedSignal, this, &HomeScreen::onPowerButtonPress); // need to change
    timer->start(1000);
}

void HomeScreen::clockAsMainFrame(QTimer *timer)
{
    AnalogClock *analogClock = new AnalogClock(ui->mainFrameUpperLeftWidget, timer, 0);
    QHBoxLayout *mainUpRightLayout = new QHBoxLayout(ui->mainFrameUppeRightWidget);
    QVBoxLayout *mainFrameLowerLayout = new QVBoxLayout(ui->mainFrameLowerWidget);

    HomeScreen::digitTime = new QLabel(ui->mainFrameUppeRightWidget);
    HomeScreen::dayOfWeek = new QLabel(ui->mainFrameLowerWidget);
    HomeScreen::Date = new QLabel(ui->mainFrameLowerWidget);

    digitTime->setAlignment(Qt::AlignCenter);
    dayOfWeek->setAlignment(Qt::AlignCenter);
    Date->setAlignment(Qt::AlignCenter);
    mainUpRightLayout->addWidget(digitTime);
    mainFrameLowerLayout->addWidget(dayOfWeek);
    mainFrameLowerLayout->addWidget(Date);
    digitTime->setStyleSheet("color: white;"
                            "font-size: 30px;");
    HomeScreen::dayOfWeek->setStyleSheet("color: white;"
                                         "font-size: 20px;");
    HomeScreen::Date->setStyleSheet("color: white;"
                                    "font-size: 32px;");
    ui->mainFrameUpperLeftWidget->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ClockPanel/ic_clock_big_base.png);");
}

void HomeScreen::clockAsSideFrame(QTimer *timer, int side)
{
    QVBoxLayout *sideLayout = new QVBoxLayout(ui->right_frame);
    HomeScreen::analogTime = new QWidget(ui->right_frame);
    AnalogClock *analogClock = new AnalogClock(analogTime, timer, 1);
    HomeScreen::digitTime = new QLabel(ui->right_frame);
    HomeScreen::dayOfWeek = new QLabel(ui->right_frame);
    HomeScreen::Date = new QLabel(ui->right_frame);

    sideLayout->setContentsMargins(15, 10, 0, 0);
    sideLayout->addWidget(analogTime, 0, Qt::AlignCenter);
    analogTime->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ClockPanel/ic_clock_base.png);"
                              "background-repeat: no-repeat;");
    analogTime->setFixedWidth(132);
    analogTime->setFixedHeight(132);
    //analogTime->setSizePolicy();
    sideLayout->addWidget(digitTime, 0, Qt::AlignTop);
    sideLayout->addWidget(dayOfWeek, 0, Qt::AlignTop);
    sideLayout->addWidget(Date, 0, Qt::AlignTop);

    digitTime->setAlignment(Qt::AlignCenter);
    digitTime->setStyleSheet("color: white;"
                            "font-size: 24px;");
    dayOfWeek->setAlignment(Qt::AlignCenter);
    dayOfWeek->setStyleSheet("color: white;"
                            "font-size: 18px;");
    Date->setAlignment(Qt::AlignCenter);
    Date->setStyleSheet("color: white;"
                        "font-size: 24px;");

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
    QString output;
    int day = QDate::currentDate().dayOfWeek();

    switch(day){
        case 1:
        output = "Monday";
        break;

        case 2:
        output = "Tuesday";
        break;

        case 3:
        output = "Wednesday";
        break;

        case 4:
        output = "Thursday";
        break;

        case 5:
        output = "Friday";
        break;

        case 6:
        output = "Saturday";
        break;

        case 7:
        output = "Sunday";
        break;

    }

    //output = QString::number(day);
    HomeScreen::dayOfWeek->setText(output);
}

void HomeScreen::displayDate(int side)
{
    QString output;
    int day = QDate::currentDate().day();
    int month = QDate::currentDate().month();
    int year = QDate::currentDate().year();

    if (day < 10){
        output = "0" + QString::number(day);
    }
    output = QString::number(day);

    switch(month){
        case 1:
        output += " January";
        break;

        case 2:
        output += " February";
        break;

        case 3:
        output += " March";
        break;

        case 4:
        output += " April";
        break;

        case 5:
        output += " May";
        break;

        case 6:
        output += " June";
        break;

        case 7:
        output += " July";
        break;

        case 8:
        output += " August";
        break;

        case 9:
        output += " September";
        break;

        case 10:
        output += " October";
        break;

        case 11:
        output += " November";
        break;

        case 12:
        output += " December";
        break;
    }
    if(side != 0){
        output.resize(6);
        output = output.toUpper();
    }
    output = output + " " + QString::number(year);
    HomeScreen::Date->setText(output);
}

QString HomeScreen::digitImageStringify(int label)
{
    QString stylesheetformat, int_to_str;

    int_to_str = QString::number(label);
    stylesheetformat = "border-image: url(:/home_screen/background/Bitmaps/home_screen/clock_font24/hours/" + int_to_str + ".png) 0 0 0 0 stretch stretch;";
    return stylesheetformat;
}

void HomeScreen::onPowerButtonPress()
{
    if (visible == true){
        visible = false;
        screenSaver->show();
        hide();

    }
    else{
        visible = true;
        show();
        screenSaver->hide();
        delete screenSaver;
    }

}
