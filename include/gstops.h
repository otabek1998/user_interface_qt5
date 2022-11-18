#ifndef GSTOPS_H
#define GSTOPS_H

#include <gst/gst.h>
#include <string>
#include <QObject>

class GstOps : public QObject
{
    Q_OBJECT
public:
    GstOps();
    void play_uri(std::string);
    void pause_music();
    void resume_music();
private:
    guint bus_watch_id;
    GMainLoop *loop;
    GstElement *pipeline;
    GstElement *sink;
    GstBus *bus;
    GstMessage *msg;
    static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data);
};

#endif // GSTOPS_H
