#ifndef GSTOPS_H
#define GSTOPS_H

#include <gst/gst.h>
#include <string>

class GstOps
{
public:
    GstOps();
    void play_music(std::string);
    void pause_music();
private:
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;
};

#endif // GSTOPS_H
