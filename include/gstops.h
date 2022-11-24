#ifndef GSTOPS_H
#define GSTOPS_H

#include <gst/gst.h>
#include <string>
#include <QObject>

typedef struct _CustomData {
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
} CustomData;


class GstOps : public QObject
{
    Q_OBJECT
public:
    GstOps();
    int play_uri(std::string);
    void pause_music();
    void resume_music();
    void volume_up();
    void volume_down();
    void changeMusic(std::string);
    CustomData *data;
private:
    guint bus_watch_id;
    GMainLoop *loop;
    //GstElement *pipeline;
    //GstElement *sink;
    //GstElement *decodebin;
    //GstElement *audioconvert;
    //GstElement *volume;
    //GstElement *source;



    static void onPadAdded(GstElement*, GstPad*, CustomData*);

    GstBus *bus;
    GstMessage *msg;
    static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data);
    static gboolean cb_print_position (GstElement* pipeline);
    static void print_one_tag(const GstTagList * list, const gchar * tag, gpointer user_data);
    static gboolean pause_resume_pipeline (CustomData* cust);
    static gboolean volume_change (CustomData* cust);
};

#endif // GSTOPS_H
