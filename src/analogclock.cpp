#include <QtGui>
#include "include/analogclock.h"

AnalogClock::AnalogClock(QWidget *parent, QTimer *timer, int side)
    : QWidget(parent)
{
    this->side = side;
    if(side != 0){
        AnalogClock::minuteHand = {0, 10, 3, -50};
        AnalogClock::hourHand = {0, 10, 3, -35};
        AnalogClock::widgetSize = 132;
        this->resize(this->widgetSize, this->widgetSize);
    }
    else{
        AnalogClock::minuteHand = {0, 10, -3, -62};
        AnalogClock::hourHand = {0, 10, -3, -40};
        AnalogClock::widgetSize = 170;
        this->resize(this->widgetSize, this->widgetSize);
    }
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void AnalogClock::paintEvent(QPaintEvent *)
{
    /*if (AnalogClock::size == 132){
        QRect minuteHand = {0, 10, -4, -50};
        QRect hourHand = {0, 10, -4, -40};
    }
    else{
        QRect minuteHand = {0, 10, -4, -62};
        QRect hourHand = {0, 10, -4, -40};
    }*/
    QColor hourColor(255, 255, 255);
    QColor minuteColor(255, 255, 255);

    int sidemin = qMin(width(), height());
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(sidemin / float(widgetSize), sidemin / float(widgetSize));
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);
    painter.save();

    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawRect(hourHand);
    painter.restore();

    painter.setPen(hourColor);

    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);

    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawRect(minuteHand);
    painter.restore();

    painter.setPen(minuteColor);
}
