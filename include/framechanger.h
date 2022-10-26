#ifndef FRAMECHANGER_H
#define FRAMECHANGER_H

#include <QWidget>
#include <QPushButton>

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
    QPushButton *resetButton;
    QPushButton *cancelButton;
    QPushButton *doneButton;
    void cancelButtonPress();
 signals:
    void emitCancelButtonPress();

private:
    Ui::FrameChanger *ui;
    void paintEvent(QPaintEvent *);
};

#endif // FRAMECHANGER_H
