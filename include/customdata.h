#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include <QObject>
#include <string>
#include <gst/gst.h>

class CustomData : public QObject {
  Q_OBJECT
public:
  GstElement *pipeline;
  GstElement *source;
  GstElement *decodebin;
  GstElement *convert;
  GstElement *volume;
  GstElement *sink;
  bool isPlaying;
  float curr_volume;
  std::string song_name;
  std::string artist;
  GMainLoop *loop;
  void setArtist(std::string);
  void setSongName(std::string);
  void setAlbumArt(GstMapInfo);
  uint8_t *album_art;
  size_t album_art_size;
signals:
   void onArtistChange();
   void onSongNameChange();
   void onAlbumArtChange();
   void onStreamEnded();
};

#endif // CUSTOMDATA_H
