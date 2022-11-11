#include "include/gstops.h"
#include <string>
#include <gst/gst.h>

GstOps::GstOps()
{
    bus = NULL;
    msg = NULL;
}

void GstOps::pause_music()
{

}

void GstOps::play_music(std::string location)
{
    int ret;
    std::string uri = "playbin uri=file://";
    uri += location;
    uri += " audio-sink=\"alsasink device=hw:1,1\"";

    const char* convert_to_gchar = uri.c_str();

    pipeline = gst_parse_launch(convert_to_gchar, NULL);

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
