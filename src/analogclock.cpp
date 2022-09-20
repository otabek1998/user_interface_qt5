#include <QtGui>
#include "include/analogclock.h"

AnalogClock::AnalogClock(QWidget *parent, QTimer *timer)
    : QWidget(parent)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    this->resize(170, 170);
}

void AnalogClock::paintEvent(QPaintEvent *)
{
    QRect minuteHand = {0, 10, 4, -62};
    QRect hourHand = {0, 10, 4, -40};
    QColor hourColor(255, 255, 255);
    QColor minuteColor(255, 255, 255);

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 170.0, side / 170.0);
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
