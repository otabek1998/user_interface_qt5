#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QString>

class MusicPlayer : public QWidget
{
    Q_OBJECT
public:
    MusicPlayer(QWidget *parent = nullptr);

private:
    QString *directoryOfUSB;

public slots:
    void openFile();
};

#endif // MUSICPLAYER_H


