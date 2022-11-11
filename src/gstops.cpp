#include "include/gstops.h"
#include <string>
#include <gst/gst.h>

GstOps::GstOps()
{
    bus = NULL;
    msg = NULL;
}

GstOps::pause_music()
{

}

GstOps::play_music(char* location)
{
    char* uri = "playbin uri=file://" + location;
    uri += " audio-sink=\"alsasink device=hw:1,1\"";

    pipeline = gst_parse_launch(uri, NULL);

    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    // free memory
    if (msg != NULL){
         gst_message_unref(msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}
