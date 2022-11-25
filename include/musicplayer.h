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
    bool isInitialized;
    int now_playing_music_index;
    int len_of_playlist;
    std::vector<std::string> *playlist;
    void pauseMusic();
    std::string getArtist();
    std::string getSongName();
    uint8_t* getAlbumCover();
    size_t getAlbumCoverSize();

    GstOps *gstops;
private:
    QString *directoryOfUSB;

public slots:
    void createPlaylist();
    void resumeMusic();
    void volumeUp();
    void volumeDown();
    void nextMusic();
    void prevMusic();
};

#endif // MUSICPLAYER_H


