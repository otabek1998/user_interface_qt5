#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QWidget>
#include "include/analogclock.h"

namespace Ui {
class ScreenSaver;
}

class ScreenSaver : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenSaver(QWidget *parent = 0);
    ~ScreenSaver();

private:
    Ui::ScreenSaver *ui;
    AnalogClock *clock;
    QWidget *analogClockWidget;
    void paintEvent(QPaintEvent *);
};

#endif // SCREENSAVER_H
