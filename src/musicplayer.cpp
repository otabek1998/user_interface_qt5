#include "include/musicplayer.h"

#include <QUrl>
#include <QDir>
#include <QDirIterator>
#include <QStringList>

MusicPlayer::MusicPlayer()
{
    this->directoryOfUSB = new QString("/media");
}

void MusicPlayer::openFile()
{
    //QMediaPlayer player;
    //QMediaPlaylist playlist;
    gstops = new GstOps();
    std::string location;
    gst_init(NULL, NULL);
    QDirIterator it(*directoryOfUSB,QStringList() << "*.mp3",QDir::Files,QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        //player.setMedia(QUrl(it.next()));
        //qDebug() << it.next();
        location = it.next().toUtf8().constData();
        //playlist.addMedia(QUrl::fromLocalFile(it.next()));
        //player.setMedia(QUrl::fromLocalFile(it.next()));
        //player.setMedia(QUrl("gst-pipeline: audiotestsrc ! alsasink"));
    }
    //player.setPlaylist(&playlist);
    //player.play();
    gstops->play_music(location);
}
