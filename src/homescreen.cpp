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
    this->setObjectName("MainMenu");
    this->setStyleSheet(widgetStyle);
}
