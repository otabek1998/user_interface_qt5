#ifndef GSTOPS_H
#define GSTOPS_H

#include <gst/gst.h>

class GstOps
{
public:
    play_music(char*);
    pause_music();
private:
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;
}

#endif // GSTOPS_H
