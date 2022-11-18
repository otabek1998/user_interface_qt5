#include "include/gstops.h"
#include <string>
#include <iostream>
#include <gst/gst.h>

GstOps::GstOps()
{
    pipeline = NULL;
    bus = NULL;
    msg = NULL;
    sink = NULL;
}

void GstOps::pause_music()
{
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
    std::cout << "Gstreamer pause" << std::endl;
}

void GstOps::resume_music()
{
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
    std::cout << "Gstreamer resume" << std::endl;
}

void GstOps::play_uri(std::string location)
{
    int ret;
    loop = g_main_loop_new(NULL, FALSE);
    std::string uri = "file://" + location;

    //std::string uri = "playbin uri=file://";
    //uri += location;
    //uri += " audio-sink=\"alsasink device=hw:0,1\"";

    const char* convert_to_gchar = uri.c_str();
 
    std::cout << convert_to_gchar << std::endl;
    //pipeline = gst_parse_launch(convert_to_gchar, NULL);
    pipeline = gst_element_factory_make("playbin", "player");
    sink = gst_element_factory_make("audio-sink", "audiosink");

    g_object_set(G_OBJECT(pipeline), "uri", convert_to_gchar, NULL);
    g_object_set(G_OBJECT(sink), "audio-sink", "alsasink device=hw:0,1", NULL);

    gst_bin_add(GST_BIN(pipeline), sink);

    std::cout << "Pipeline is created" << std::endl;
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref(bus);
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    g_main_loop_run(loop);

    // free memory
    if (msg != NULL){
         gst_message_unref(msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

gboolean GstOps::bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}
