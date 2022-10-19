#include "include/customframe.h"
#include <QMouseEvent>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>

CustomFrame::CustomFrame(QWidget *parent) : QWidget(parent)
{
    grabGesture(Qt::TapAndHoldGesture);
    QTapAndHoldGesture::setTimeout(1300);
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

/*void CustomFrame::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        emit pressedSignal();
    }
}*/

bool CustomFrame::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture){
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QWidget::event(event);
}

bool CustomFrame::gestureEvent(QGestureEvent *event)
{
    if (QGesture *tapandhold = event->gesture(Qt::TapAndHoldGesture)){
        tapandholdTriggered(static_cast<QTapAndHoldGesture *>(tapandhold));
    }
    return true;
}

void CustomFrame::tapandholdTriggered(QTapAndHoldGesture *gesture)
{

    if (gesture->state() == Qt::GestureFinished){
        emit pressedSignal();
    }
}
