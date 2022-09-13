#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>

namespace Ui {
class HomeScreen;
}

class HomeScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = 0);
    ~HomeScreen();
private:
    Ui::HomeScreen *ui;
    void Initialize();
    QString digitImageStringify(int);
private slots:
    void showTime();
};

#endif // HOMESCREEN_H
