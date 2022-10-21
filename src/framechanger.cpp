#include "framechanger.h"
#include "ui_framechanger.h"

FrameChanger::FrameChanger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameChanger)
{
    ui->setupUi(this);
}

void FrameChanger::Initialize()
{

}

FrameChanger::~FrameChanger()
{
    delete ui;
}
