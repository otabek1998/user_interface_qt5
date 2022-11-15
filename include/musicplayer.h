#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "include/gstops.h"
#include <QObject>
#include <QString>
#include <vector>

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    MusicPlayer();
    bool isPlaying;
    int now_playing_music_index;
    int len_of_playlist;
    std::vector<std::string> *playlist;
    void playMusic();
    void pauseMusic();
    void resumeMusic();
    void nextMusic();
    void prevMusic();
private:
    QString *directoryOfUSB;
    GstOps *gstops;
public slots:
    void createPlaylist();
};

#endif // MUSICPLAYER_H


