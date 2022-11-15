#include "include/musicplayer.h"

#include <QUrl>
#include <QDir>
#include <QDirIterator>
#include <QStringList>
#include <iostream>

MusicPlayer::MusicPlayer()
{
    this->directoryOfUSB = new QString("/run");
    isPlaying = false;
}

void MusicPlayer::createPlaylist()
{
    gstops = new GstOps();
    std::string location;
    playlist = new std::vector<std::string>;

    gst_init(NULL, NULL);
    QDirIterator it(*directoryOfUSB,QStringList() << "*.mp3",QDir::Files,QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        //player.setMedia(QUrl(it.next()));
        //qDebug() << it.next();
        location = it.next().toUtf8().constData();
        std::cout << location << std::endl;
        playlist->push_back(location);
    }
    this->len_of_playlist = playlist->size() - 1;
    std::cout << len_of_playlist << std::endl;
    if (playlist->size() > 0){
        this->now_playing_music_index = 0;
    }
}

void MusicPlayer::playMusic()
{
    if (isPlaying == false){
        isPlaying = true;
        gstops->play_music(playlist->at(now_playing_music_index));
    }
    else {
        isPlaying == true;
        pauseMusic();
    }
}

void MusicPlayer::pauseMusic()
{
    if (isPlaying == true){
        isPlaying = false;
        gstops->pause_music();
    }
    else {
        qDebug("Error! music is already paused");
    }

}

void MusicPlayer::resumeMusic()
{
    if (isPlaying == false) {
        isPlaying = true;
        gstops->resume_music();
    }
    else {
        qDebug("Error music is already playing");
    }
}
