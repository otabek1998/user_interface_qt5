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
    void play_music(std::string);
    void pause_music();
    void resume_music();
private:
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;
};

#endif // GSTOPS_H
