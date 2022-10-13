#include "include/customframe.h"
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>

CustomFrame::CustomFrame(QWidget *parent) : QWidget(parent)
{

}

CustomFrame::~CustomFrame()
{

}

void CustomFrame::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CustomFrame::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit pressedSignal();
    }
}

