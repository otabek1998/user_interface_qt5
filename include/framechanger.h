#ifndef FRAMECHANGER_H
#define FRAMECHANGER_H

#include <QWidget>

namespace Ui {
class FrameChanger;
}

class FrameChanger : public QWidget
{
    Q_OBJECT

public:
    explicit FrameChanger(QWidget *parent = 0);
    ~FrameChanger();
    void Initialize();

private:
    Ui::FrameChanger *ui;
    void paintEvent(QPaintEvent *);
};

#endif // FRAMECHANGER_H
