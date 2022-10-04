#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QWidget>

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
};

#endif // SCREENSAVER_H
