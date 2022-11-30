#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <qpainter.h>
#include <unistd.h>
#include <iostream>
#include <QBitmap>
#include "include/homescreen.h"
#include "include/analogclock.h"
#include "include/touchinput.h"
#include "include/customframe.h"
#include "include/musicplayer.h"
#include "ui_homescreen.h"

HomeScreen::HomeScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeScreen)
{
    ui->setupUi(this);
    setLeftFrameContent("vehicle_data");
    setRightFrameContent("clock");
    setMainFrameContent("player");
    this->Initialize();
    visible = true;
    //frame_contents[0] = "clock";
    //frame_contents[1] = "vehicle_data";
    //frame_contents[2] = "player";
}

HomeScreen::~HomeScreen()
{
    delete ui;
}

void HomeScreen::setAllFrames()
{
    for (int i = 0; i < 2; i++)
    {
        if (frame_array[i] == "vehicle_data"){
            vehicleDataAsSideFrame(i);
        }
        else if (frame_array[i] == "player"){
            playerAsSideFrame(i);
        }
        else {
            clockAsSideFrame(timer, i);
        }
    }

    if (frame_array[2] == "vehicle_data") {
        vehicleDataAsMainFrame();
    }
    else if (frame_array[2] == "player") {
        playerAsMainFrame();
    }
    else if (frame_array[2] == "clock") {
        clockAsMainFrame(timer);
    }
}

void HomeScreen::resetAllFrames()
{
    playerAsMainFrame();
    vehicleDataAsSideFrame(0);
    clockAsSideFrame(timer, 1);
    fc->hide();
    show();
}

void HomeScreen::Initialize()
{
    timer = new QTimer(this);
    screenSaver = new ScreenSaver();
    player = new MusicPlayer;
    fc = new FrameChanger();
    ti = new TouchInput();
    ti_thread = new QThread();
    ti->moveToThread(ti_thread);

    QString widgetStyle = "QWidget#MainMenu {"
                          "background-image: url(:/home_screen/background/Bitmaps/home_screen/0569-bg_1_home.png);"
                          "background-repeat: no-repeat}";
    QString sideWidgetStyle = "CustomFrame#SideFrame {"
                              "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_small.png);"
                              "background-repeat: no-repeat}";

    /*clockAsMainFrame(timer);
    //vehicleDataAsMainFrame();
    //playerAsMainFrame();
    playerAsSideFrame(1);
    //clockAsSideFrame(timer, 1);
    vehicleDataAsSideFrame(0);*/

    setAllFrames();

    showTimeOnStatusBar();
    showTimeOnMainFrame();
    displayDayOfWeek();
    displayDate(0);
    displayDate(1);


    //player->createPlaylist();
    this->setObjectName("MainMenu");
    ui->mainFrame->setObjectName("MainFrame");
    ui->left_frame->setObjectName("LeftFrame");
    ui->right_frame->setObjectName("RightFrame");
    CustomFrame *frame = this->findChild<CustomFrame*>("MainFrame"); // taking object from ui
    CustomFrame *left_frame = this->findChild<CustomFrame*>("LeftFrame");
    CustomFrame *right_frame = this->findChild<CustomFrame*>("RightFrame");
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

    connect(timer, SIGNAL(timeout()), this, SLOT(showTimeOnStatusBar()));
    connect(timer, SIGNAL(timeout()), this, SLOT(showTimeOnMainFrame()));
    connect(ti_thread, &QThread::started, ti, &TouchInput::Process);
    connect(ti, &TouchInput::emitPowerButtonSignal, this, &HomeScreen::onPowerButtonPress);
    connect(ti, &TouchInput::emitVolumeUpSignal, player, &MusicPlayer::volumeUp);
    connect(ti, &TouchInput::emitVolumeDownSignal, player, &MusicPlayer::volumeDown);
    connect(frame, &CustomFrame::pressedSignal, this, &HomeScreen::onFrameHoldGesture); // need to change
    connect(left_frame, &CustomFrame::pressedSignal, this, &HomeScreen::onFrameHoldGesture); // need to change
    connect(right_frame, &CustomFrame::pressedSignal, this, &HomeScreen::onFrameHoldGesture); // need to change
    connect(fc, &FrameChanger::emitCancelButtonPress, this, &HomeScreen::onFrameChangerCancelPress);
    connect(fc, &FrameChanger::emitResetButtonPress, this, &HomeScreen::resetAllFrames);
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
    QVBoxLayout *sideLayout;
    AnalogClock *analogClock;
    if (side == 1)
    {
        sideLayout = new QVBoxLayout(ui->right_frame);
        HomeScreen::analogTime = new QWidget(ui->right_frame);
        analogClock = new AnalogClock(analogTime, timer, 1);
        HomeScreen::digitTime = new QLabel(ui->right_frame);
        HomeScreen::dayOfWeek = new QLabel(ui->right_frame);
        HomeScreen::Date = new QLabel(ui->right_frame);
        sideLayout->setContentsMargins(15, 10, 0, 0);
    }
    else
    {
        sideLayout = new QVBoxLayout(ui->left_frame);
        HomeScreen::analogTime = new QWidget(ui->left_frame);
        analogClock = new AnalogClock(analogTime, timer, 1);
        HomeScreen::digitTime = new QLabel(ui->left_frame);
        HomeScreen::dayOfWeek = new QLabel(ui->left_frame);
        HomeScreen::Date = new QLabel(ui->left_frame);
        sideLayout->setContentsMargins(0, 10, 15, 0);
    }

    sideLayout->addWidget(analogTime, 0, Qt::AlignCenter);
    analogTime->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ClockPanel/ic_clock_base.png);"
                              "background-repeat: no-repeat;"
                              "background-position: center;");
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

void HomeScreen::vehicleDataAsMainFrame()
{
    QVBoxLayout *upperLayout = new QVBoxLayout(ui->mainFrameUppeRightWidget);
    QVBoxLayout *lowerLayout = new QVBoxLayout(ui->mainFrameLowerWidget);
    QLabel *average = new QLabel(ui->mainFrameUppeRightWidget);
    QLabel *averageValue = new QLabel(ui->mainFrameUppeRightWidget);
    QWidget *rangeWidget = new QWidget(ui->mainFrameLowerWidget);
    QLabel *rangeValue = new QLabel(ui->mainFrameLowerWidget);

    upperLayout->addWidget(average, 0, Qt::AlignBottom);
    upperLayout->addWidget(averageValue, 0, Qt::AlignTop);

    lowerLayout->addWidget(rangeWidget, 0, Qt::AlignCenter);
    lowerLayout->addWidget(rangeValue, 0, Qt::AlignTop);

    rangeWidget->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_range_fuel.png);"
                               "background-repeat: no-repeat;");
    rangeWidget->setFixedSize(54, 38);

    rangeValue->setAlignment(Qt::AlignCenter);
    rangeValue->setText("***km");
    rangeValue->setStyleSheet("color: white;"
                                "font-size: 24px;");

    averageValue->setAlignment(Qt::AlignCenter);
    averageValue->setText("**.*km/L");
    averageValue->setStyleSheet("color: white;"
                                "font-size: 24px;");

    average->setAlignment(Qt::AlignCenter);
    average->setText("Average");
    average->setStyleSheet("color: #03A5FC;"
                           "font-size: 20px;");
    ui->mainFrameUpperLeftWidget->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/art_vehicle_info_fuel.png);");
}

void HomeScreen::vehicleDataAsSideFrame(int side)
{
    QVBoxLayout *sideLayout;
    QWidget *fuelWidget;
    QLabel *fuelUsage;
    QWidget *rangeWidget;
    QLabel *range;
    if (side == 0)
    {
        sideLayout = new QVBoxLayout(ui->left_frame);
        fuelWidget = new QWidget(ui->left_frame);
        fuelUsage = new QLabel(ui->left_frame);
        rangeWidget = new QWidget(ui->left_frame);
        range = new QLabel(ui->left_frame);
        sideLayout->setContentsMargins(0, 10, 15, 0);
    }
    else {
        sideLayout = new QVBoxLayout(ui->right_frame);
        fuelWidget = new QWidget(ui->right_frame);
        fuelUsage = new QLabel(ui->right_frame);
        rangeWidget = new QWidget(ui->right_frame);
        range = new QLabel(ui->right_frame);
        sideLayout->setContentsMargins(15, 10, 0, 0);
    }

    sideLayout->addWidget(fuelWidget, 0, Qt::AlignCenter);
    sideLayout->addWidget(fuelUsage, 0, Qt::AlignTop);
    sideLayout->addWidget(rangeWidget, 0, Qt::AlignCenter);
    sideLayout->addWidget(range, 0, Qt::AlignTop);
    fuelUsage->setAlignment(Qt::AlignCenter);
    range->setAlignment(Qt::AlignCenter);

    fuelUsage->setText("**.*");
    fuelUsage->setStyleSheet("color: white;"
                             "font-size: 32px");
    range->setText("***");
    range->setStyleSheet("color: white;"
                         "font-size: 32px");

    fuelWidget->setFixedHeight(100);
    fuelWidget->setFixedWidth(100);
    rangeWidget->setFixedSize(54, 38);
    fuelWidget->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_fuel_small.png);"
                              "background-repeat: no-repeat;");
    rangeWidget->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_range_fuel.png);"
                               "background-repeat: no-repeat;");

}

void HomeScreen::playerAsMainFrame()
{
    QVBoxLayout *sideLayout = new QVBoxLayout(ui->mainFrameUppeRightWidget);
    QHBoxLayout *downLayout = new QHBoxLayout(ui->mainFrameLowerWidget);
    play = new QPushButton(ui->mainFrameLowerWidget);
    next = new QPushButton(ui->mainFrameLowerWidget);
    previous = new QPushButton(ui->mainFrameLowerWidget);
    songName = new QLabel(ui->mainFrameUppeRightWidget);
    artistName = new QLabel(ui->mainFrameUppeRightWidget);
    QLabel *musicTime = new QLabel(ui->mainFrameUppeRightWidget); //this should be made class member later
    //QWidget *songProgress new QWidget(ui->mainFrameUppeRightWidget);

    downLayout->addWidget(previous, 0, Qt::AlignCenter);
    downLayout->addWidget(play, 0, Qt::AlignCenter);
    downLayout->addWidget(next, 0, Qt::AlignCenter);

    play->setFixedSize(100,100);
    previous->setFixedSize(80,80);
    next->setFixedSize(80,80);

    sideLayout->addWidget(songName, 0, Qt::AlignTop);
    sideLayout->addWidget(artistName, 0, Qt::AlignTop);
    sideLayout->addWidget(musicTime, 0, Qt::AlignTop);
    songName->setText("Unknown");
    artistName->setText("Unknown");
    songName->setStyleSheet("color: white;"
                            "font-size: 32px");
    artistName->setStyleSheet("color: white;"
                              "font-size: 24px");
    musicTime->setText("--/--");
    musicTime->setStyleSheet("color: white");

    ui->mainFrameUpperLeftWidget->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/art_album_default.png);");
    play->setStyleSheet("QPushButton {"
                         "background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_play.png);"
                         "background-repeat: no-repeat;"
                         "background-color: transparent;"
                         "background-position: center;}");

    //pause->setStyleSheet("border-radius: 5px");
    previous->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_autoSeek_down.png);"
                            "background-repeat: no-repeat;"
                            "background-color: transparent;"
                            "background-position: center;");
    next->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_autoSeek_up.png);"
                        "background-repeat: no-repeat;"
                        "background-color: transparent;"
                        "background-position: center;");
    play->setFlat(true);
    next->setFlat(true);
    previous->setFlat(true);
    play->setFocusPolicy(Qt::NoFocus);
    next->setFocusPolicy(Qt::NoFocus);
    previous->setFocusPolicy(Qt::NoFocus);
    connect(play, SIGNAL(clicked()), player, SLOT(resumeMusic()));
    connect(next, SIGNAL(clicked()), player, SLOT(nextMusic()));
    connect(previous, SIGNAL(clicked()), player, SLOT(prevMusic()));
    connect(player->gstops->data, SIGNAL(onArtistChange()), this, SLOT(setArtistOnMainFrame()));
    connect(player->gstops->data, SIGNAL(onSongNameChange()), this, SLOT(setSongNameOnMainFrame()));
    connect(player->gstops->data, SIGNAL(onAlbumArtChange()),this, SLOT(setAlbumArtOnMainFrame()));

}

void HomeScreen::playerAsSideFrame(int side)
{
    QVBoxLayout *sideLayout;
    if (side == 0){
        sideLayout = new QVBoxLayout(ui->left_frame);
    }
    else {
        sideLayout = new QVBoxLayout(ui->right_frame);
    }
    playOnSideFrame = new QPushButton();
    songNameOnSideFrame = new QLabel();
    sourceWidgetOnSideFrame = new QWidget();

    sideLayout->addWidget(sourceWidgetOnSideFrame, 0, Qt::AlignCenter);
    sideLayout->addWidget(songNameOnSideFrame, 0, Qt::AlignCenter);
    sideLayout->addWidget(playOnSideFrame, 0, Qt::AlignCenter);

    sourceWidgetOnSideFrame->setStyleSheet("background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_source_usb_large.png);");
    sourceWidgetOnSideFrame->setFixedSize(84,84);

    songNameOnSideFrame->setText("Unknown");
    songNameOnSideFrame->setStyleSheet("color: white;"
                                       "font-size: 20px;");
    songNameOnSideFrame->setMargin(12);

    playOnSideFrame->setFixedSize(100,100);
    playOnSideFrame->setStyleSheet("QPushButton {"
                         "background-image: url(:/home_screen/background/Bitmaps/home_screen/ic_play.png);"
                         "background-repeat: no-repeat;"
                         "background-color: transparent;"
                         "background-position: center;}");
    playOnSideFrame->setFlat(true);
    playOnSideFrame->setFocusPolicy(Qt::NoFocus);
    connect(playOnSideFrame, SIGNAL(clicked()), player, SLOT(resumeMusic()));
    connect(player->gstops->data, SIGNAL(onSongNameChange()), this, SLOT(setSongNameOnSideFrame()));
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
        screenSaver->showFullScreen();
        hide();

    }
    else{
        visible = true;
        showFullScreen();
        screenSaver->hide();
    }

}

void HomeScreen::onFrameHoldGesture()
{
    fc->showFullScreen();
    hide();
}

void HomeScreen::onFrameChangerCancelPress()
{
    fc->hide();
    showFullScreen();
}

void HomeScreen::setArtistOnMainFrame()
{
    QString str;
    str = QString::fromStdString(player->getArtist());
    artistName->setText(str);
}

void HomeScreen::setSongNameOnMainFrame()
{
    QString str;
    str = QString::fromStdString(player->getSongName());
    songName->setText(str);
}

void HomeScreen::setSongNameOnSideFrame()
{
    QString str;
    str = QString::fromStdString(player->getSongName());
    songNameOnSideFrame->setText(str);
}

void HomeScreen::setAlbumArtOnMainFrame()
{
    QPixmap img;
    QBitmap map(170,170);

    img.loadFromData(reinterpret_cast<const uchar*>(player->getAlbumCover()), player->getAlbumCoverSize());
    img = img.scaled(170,170,Qt::KeepAspectRatio);
    //QPainter painter (&map);
    //painter.setBrush(Qt::color1);
    //painter.drawRoundRect(0,0,170,170,20,20);
    //img.setMask(map);
    ui->label->setPixmap(img);
    //ui->label->setStyleSheet("QLabel{border-radius:25px}");
}

void HomeScreen::setLeftFrameContent(std::string str)
{
    frame_array[0] = str;
}

void HomeScreen::setRightFrameContent(std::string str)
{
    frame_array[1] = str;
}

void HomeScreen::setMainFrameContent(std::string str)
{
    frame_array[2] = str;
}

std::string HomeScreen::getLeftFrameContent()
{
    return frame_array[0];
}

std::string HomeScreen::getRightFrameContent()
{
    return frame_array[1];
}

std::string HomeScreen::getMainFrameContent()
{
    return frame_array[2];
}
