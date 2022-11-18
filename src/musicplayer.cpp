#include "include/musicplayer.h"

#include <QUrl>
#include <QDir>
#include <QDirIterator>
#include <QStringList>
#include <iostream>

MusicPlayer::MusicPlayer()
{
    this->directoryOfUSB = new QString("/media");
    isPlaying = false;
    isInitialized = false;
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


void MusicPlayer::pauseMusic()
{
    isPlaying = false;
    gstops->pause_music();
}

void MusicPlayer::resumeMusic()
{
    std::cout << "Resume music is called" << std::endl;
    if (isInitialized == false){
        std::cout << "1" << std::endl;
        gstops->play_uri(playlist->at(now_playing_music_index));
        isPlaying = true;
        isInitialized = true;
    }
    else if (isPlaying == false){
        std::cout << "2" << std::endl;
        isPlaying = true;
        gstops->resume_music();
    }
    else {
        std::cout << "3" << std::endl;
        gstops->pause_music();
        isPlaying = false;
    }
}
