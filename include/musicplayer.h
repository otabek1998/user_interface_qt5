#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "include/gstops.h"
#include <QObject>
#include <QString>

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    MusicPlayer();

private:
    QString *directoryOfUSB;
    GstOps *gstops;

public slots:
    void openFile();
};

#endif // MUSICPLAYER_H


