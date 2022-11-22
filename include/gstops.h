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
} CustomData;

class GstOps : public QObject
{
    Q_OBJECT
public:
    GstOps();
    int play_uri(std::string);
    void pause_music();
    void resume_music();
private:
    guint bus_watch_id;
    GMainLoop *loop;
    //GstElement *pipeline;
    //GstElement *sink;
    //GstElement *decodebin;
    //GstElement *audioconvert;
    //GstElement *volume;
    //GstElement *source;

    CustomData *data;

    static void onPadAdded(GstElement*, GstPad*, CustomData*);

    //void onPadAdded(decodebin, );

    GstBus *bus;
    GstMessage *msg;
    static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data);
    static gboolean cb_print_position (GstElement* pipeline);
    static gboolean pause_resume_pipeline (CustomData* cust);
};

#endif // GSTOPS_H
