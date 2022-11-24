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
signals:
   void onArtistChange();
   void onSongNameChange();
};

#endif // CUSTOMDATA_H