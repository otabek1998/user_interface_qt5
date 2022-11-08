#include "include/musicplayer.h"
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
    QDirIterator it(*directoryOfUSB,QStringList() << "*.mp3",QDir::Files,QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        //player.setMedia(QUrl(it.next()));
        qDebug() << it.next();
        playlist.addMedia(QUrl::fromLocalFile(it.next()));
        player.setMedia(QUrl::fromLocalFile(it.next()));
    }
    //player.setPlaylist(&playlist);
    player.play();
}
