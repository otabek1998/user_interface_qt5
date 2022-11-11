#include "include/musicplayer.h"
#include "include/gstops.h"
#include <QUrl>
#include <QMediaContent>
#include <QDir>
#include <QDirIterator>
#include <QStringList>
#include <QMediaPlaylist>

MusicPlayer::MusicPlayer(QWidget *parent) : QWidget(parent)
{
    this->directoryOfUSB = new QString("/run");
}

void MusicPlayer::openFile()
{
    QMediaPlayer player;
    QMediaPlaylist playlist;
    GstOps *gstClass;
    std::string location;
    QDirIterator it(*directoryOfUSB,QStringList() << "*.mp3",QDir::Files,QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        //player.setMedia(QUrl(it.next()));
        qDebug() << it.next();
        location = it.next().toUtf8().constData();
        //playlist.addMedia(QUrl::fromLocalFile(it.next()));
        //player.setMedia(QUrl::fromLocalFile(it.next()));
    }
    //player.setPlaylist(&playlist);
    //player.play();
    gstClass->play_music(location);
}
