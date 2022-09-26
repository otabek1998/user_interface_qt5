#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QWidget>

//! [0]
class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    AnalogClock(QWidget *parent = 0);
    AnalogClock(QWidget *parent, QTimer *timer, int side);
private:
    int side; // side can be 0 for main frame, 1 or 2 for side frames (1 for right, 2 for left)
    int widgetSize; // widgetSize is size of clock widget (height = width), side frame => height = 132, main frame => height = 170
    QRect minuteHand;
    QRect hourHand;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif
