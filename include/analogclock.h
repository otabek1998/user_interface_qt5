#ifndef ANALOGCLOCK_H
#define ANALOGCLOCK_H

#include <QWidget>

//! [0]
class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    AnalogClock(QWidget *parent = 0);
    AnalogClock(QWidget *parent, QTimer *timer);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif
