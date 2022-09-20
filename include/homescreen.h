#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class HomeScreen;
}

class HomeScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = 0);
    ~HomeScreen();
    void displayClockAsMainFrame();
    QString digitImageStringify(int);
private:
    Ui::HomeScreen *ui;
    void Initialize();
    void displayDayOfWeek();

    QLabel *digitTime;
    QLabel *dayOfWeek;
    QLabel *Date;
private slots:
    void showTimeOnStatusBar();
    void showTimeOnMainFrame();
};

#endif // HOMESCREEN_H
