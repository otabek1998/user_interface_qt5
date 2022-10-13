#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include <QWidget>

class CustomFrame : public QWidget
{
    Q_OBJECT
public:
    CustomFrame(QWidget *parent = 0);
    ~CustomFrame();
private:
    void paintEvent(QPaintEvent *);

signals:
    void pressedSignal();

private slots:
    void mousePressEvent(QMouseEvent *e) override;
};

#endif // CUSTOMFRAME_H
