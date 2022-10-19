#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include <QWidget>
#include <QTapAndHoldGesture>
#include <QGestureEvent>

class CustomFrame : public QWidget
{
    Q_OBJECT
public:
    CustomFrame(QWidget *parent = 0);
    ~CustomFrame();
private:
    void paintEvent(QPaintEvent *);
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);
    void tapandholdTriggered(QTapAndHoldGesture *gesture);
    //void mousePressEvent(QMouseEvent *e) override;

signals:
    void pressedSignal();

};

#endif // CUSTOMFRAME_H
