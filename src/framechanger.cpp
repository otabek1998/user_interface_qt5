#include "include/framechanger.h"
#include "ui_framechanger.h"
#include <QPainter>
#include <iostream>

FrameChanger::FrameChanger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameChanger)
{
    ui->setupUi(this);
    this->Initialize();
}

void FrameChanger::Initialize()
{
    QString widgetStyle = "QWidget#FrameChanger {"
                          "background-image: url(:/home_screen/background/Bitmaps/home_screen/0569-bg_1_home.png);"
                          "background-repeat: no-repeat}";
    QString leftWidgetStyle = "QWidget#leftFrame {"
                              "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_small.png);"
                              "background-repeat: no-repeat}";
    QString centralWidgetStyle = "QWidget#centralFrame {"
                              "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_small.png);"
                              "background-repeat: no-repeat}";
    QString rightWidgetStyle = "QWidget#rightFrame {"
                              "background-image: url(:/home_screen/background/Bitmaps/home_screen/bg_panel_small.png);"
                              "background-repeat: no-repeat}";
    ui->cancel_button->setObjectName("CancelButton");
    ui->done_button->setObjectName("DoneButton");
    ui->reset_button->setObjectName("ResetButton");
    this->setObjectName("FrameChanger");
    this->setStyleSheet(widgetStyle);
    ui->widget->setObjectName("leftFrame");
    ui->widget_2->setObjectName("centralFrame");
    ui->widget_3->setObjectName("rightFrame");

    ui->widget->setStyleSheet(leftWidgetStyle);
    ui->widget_2->setStyleSheet(centralWidgetStyle);
    ui->widget_3->setStyleSheet(rightWidgetStyle);

    cancelButton = this->findChild<QPushButton*>("CancelButton");
    doneButton = this->findChild<QPushButton*>("DoneButton");
    resetButton = this->findChild<QPushButton*>("ResetButton");

    connect(cancelButton, &QPushButton::clicked, this, &FrameChanger::cancelButtonPress);
    connect(resetButton, &QPushButton::clicked, this, &FrameChanger::resetButtonPress);
}

FrameChanger::~FrameChanger()
{
    delete ui;
}

void FrameChanger::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FrameChanger::cancelButtonPress()
{
    emit emitCancelButtonPress();
}

void FrameChanger::resetButtonPress()
{
    emit emitResetButtonPress();
}

/*void FrameChanger::setContents(std::string arr[3])
{

}*/

