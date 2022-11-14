#include "include/gstops.h"
#include <string>
#include <iostream>
#include <gst/gst.h>

GstOps::GstOps()
{
    pipeline = NULL;
    bus = NULL;
    msg = NULL;
}

void GstOps::pause_music()
{

}

void GstOps::play_music(std::string location)
{
    int ret;
    GError* err = NULL;
    std::string uri = "playbin uri=file://";
    uri += location;
    uri += " audio-sink=\"alsasink device=hw:0,0\"";

    const char* convert_to_gchar = uri.c_str();
 
    //pipeline = gst_parse_launch("playbin uri=file:///media/otabek/B63D-28CC/videoplayback.mp3", &err);
    std::cout << convert_to_gchar << std::endl;
    pipeline = gst_parse_launch(convert_to_gchar, &err);
    //pipeline = gst_element_factory_make("playbin", "playbin");

    std::cout << "Pipeline is created" << std::endl;

    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    std::cout << ret << std::endl;
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
