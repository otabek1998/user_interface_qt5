#include "include/screensaver.h"
#include "ui_screensaver.h"

ScreenSaver::ScreenSaver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSaver)
{
    ui->setupUi(this);
    this->setObjectName("MainMenu");
    this->setStyleSheet("QWidget#MainMenu{"
                        "background-image: url(:/home_screen/background/Bitmaps/screensavers/0571-bg_1_home.png);}");
    this->showFullScreen();

}

ScreenSaver::~ScreenSaver()
{
    delete ui;
}
