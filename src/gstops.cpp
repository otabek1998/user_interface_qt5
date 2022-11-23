#include "include/gstops.h"
#include <string>
#include <iostream>
#include <gst/gst.h>

GstOps::GstOps()
{
    data = new CustomData;
}

int GstOps::play_uri(std::string location)
{
    loop = g_main_loop_new(NULL, FALSE);
    const char* convert_to_gchar = location.c_str();
    //GstBus *bus;
    //GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    /* Initialize GStreamer */
    gst_init (NULL, NULL);

    /* Create the elements */
    data->source = gst_element_factory_make("filesrc", "src");
    data->decodebin = gst_element_factory_make ("decodebin", "source");
    data->convert = gst_element_factory_make ("audioconvert", "convert");
    data->volume = gst_element_factory_make ("volume", "volume");
    data->sink = gst_element_factory_make ("alsasink", "sink");
    data->curr_volume = 0.4;

    /* Create the empty pipeline */
    data->pipeline = gst_pipeline_new ("test-pipeline");

    if (!data->pipeline || !data->source || !data->convert || !data->sink || !data->decodebin) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    /* Build the pipeline. Note that we are NOT linking the source at this
    * point. We will do it later. */
    gst_bin_add_many (GST_BIN (data->pipeline), data->source, data->decodebin, data->convert , data->volume, data->sink, NULL);
    if (!gst_element_link (data->volume, data->sink)) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (data->pipeline);
    return -1;
    }
    if (!gst_element_link (data->source, data->decodebin)) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (data->pipeline);
        return -1;
    }

    if (!gst_element_link (data->convert, data->volume)) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (data->pipeline);
        return -1;
    }

    /* Set the location to play */
    g_object_set(data->source, "location", convert_to_gchar, NULL);
    g_object_set(data->sink, "device", "hw:0", NULL);
    g_object_set(data->volume, "volume", data->curr_volume, NULL);

    /* Connect to the pad-added signal */
    g_signal_connect (data->decodebin, "pad-added", G_CALLBACK (onPadAdded), data);

    /* Start playing */
    ret = gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (data->pipeline);
        return -1;
    }

//    bus = gst_element_get_bus (data->pipeline);
//      do {
//        msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GstMessageType(GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

//        /* Parse message */
//        if (msg != NULL) {
//          GError *err;
//          gchar *debug_info;

//          switch (GST_MESSAGE_TYPE (msg)) {
//            case GST_MESSAGE_ERROR:
//              gst_message_parse_error (msg, &err, &debug_info);
//              g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
//              g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
//              g_clear_error (&err);
//              g_free (debug_info);
//              terminate = TRUE;
//              break;
//            case GST_MESSAGE_EOS:
//              g_print ("End-Of-Stream reached.\n");
//              terminate = TRUE;
//              break;
//            case GST_MESSAGE_STATE_CHANGED:
//              /* We are only interested in state-changed messages from the pipeline */
//              if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->pipeline)) {
//                GstState old_state, new_state, pending_state;
//                gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
//                g_print ("Pipeline state changed from %s to %s:\n",
//                    gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
//              }
//              break;
//            default:
//              /* We should not reach here */
//              g_printerr ("Unexpected message received.\n");
//              break;
//          }
//          gst_message_unref (msg);
//        }
//      } while (!terminate);


    bus = gst_pipeline_get_bus(GST_PIPELINE(data->pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref(bus);
    gst_element_set_state(GST_ELEMENT (data->pipeline), GST_STATE_PLAYING);
    data->isPlaying = true;

    //g_timeout_add (200, (GSourceFunc) cb_print_position, data->pipeline);
    //g_timeout_add(100, (GSourceFunc) pause_resume_pipeline, data);
    //g_timeout_add(100, (GSourceFunc) volume_change, data);


    g_main_loop_run(loop);

}

/*void GstOps::play_uri(std::string location)
{
    gst_init(NULL, NULL);

    loop = g_main_loop_new(NULL, FALSE);
    const char* convert_to_gchar = location.c_str();
    pipeline = gst_pipeline_new("pipeline");
    source = gst_element_factory_make("filesrc", "source");
    decodebin = gst_element_factory_make("decodebin", "decodebin");
    audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
    volume = gst_element_factory_make("volume", "volume");
    sink = gst_element_factory_make("alsasink", "sink");

    if(!pipeline || !source || !decodebin || !audioconvert || !volume || !sink){
        g_printerr ("Not all elements could be created.\n");
    }


    g_object_set(G_OBJECT (source), "location", convert_to_gchar, NULL);
    //g_object_set(G_OBJECT (volume), "volume", 0,1, NULL);
    //g_object_set(G_OBJECT (sink), "device", "hw:0", NULL);

    g_signal_connect (decodebin, "pad-added", G_CALLBACK (onPadAdded), &audioconvert);

    gst_bin_add_many(GST_BIN (pipeline), source, decodebin, audioconvert, volume, sink, NULL);

    gst_element_link(source, decodebin);
    gst_element_link(audioconvert, volume);
    gst_element_link(volume, sink);
    gst_element_link_pads(decodebin, "decodebin", audioconvert, "audioconvert"); //continue work here


    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref(bus);
    gst_element_set_state(GST_ELEMENT (pipeline), GST_STATE_PLAYING);

    g_main_loop_run(loop);

    if (msg != NULL){
         gst_message_unref(msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}*/

/*void GstOps::play_uri(std::string location)
{
    int ret;
    loop = g_main_loop_new(NULL, FALSE);
    std::string uri = "file://" + location;

    std::string uri = "playbin uri=file://";
    uri += location;
    uri += " audio-sink=\"alsasink device=hw:0,1\"";

    const char* convert_to_gchar = uri.c_str();
 
    std::cout << convert_to_gchar << std::endl;
    pipeline = gst_parse_launch(convert_to_gchar, NULL);

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
}*/

gboolean GstOps::bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *) data;
  GError *err;
  gchar *debug_info;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gst_message_parse_error (msg, &err, &debug_info);
      g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
      g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error (&err);
      g_free (debug_info);

      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_STATE_CHANGED:
      /* We are only interested in state-changed messages from the pipeline */
      //if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->pipeline)) {
          GstState old_state, new_state, pending_state;
          gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
          //g_print ("Pipeline state changed from %s to %s:\n",
          //gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
        //}
       break;
    }
    default:
      break;
  }

  return TRUE;
}

void GstOps::onPadAdded(GstElement *src, GstPad *new_pad, CustomData *data)
{
    GstPad *sink_pad = gst_element_get_static_pad (data->convert, "sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    const gchar *new_pad_type = NULL;

    g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

    /* If our converter is already linked, we have nothing to do here */
    if (gst_pad_is_linked (sink_pad)) {
      g_print ("  We are already linked. Ignoring.\n");
      goto exit;
    }

    /* Attempt the link */
    ret = gst_pad_link (new_pad, sink_pad);
    if (GST_PAD_LINK_FAILED (ret)) {
      g_print ("  Type is '%s' but link failed.\n", new_pad_type);
    } else {
      g_print ("  Link succeeded (type '%s').\n", new_pad_type);
    }

  exit:
    /* Unreference the new pad's caps, if we got them */
    if (new_pad_caps != NULL)
      gst_caps_unref (new_pad_caps);

    /* Unreference the sink pad */
    gst_object_unref (sink_pad);
}

gboolean GstOps::cb_print_position(GstElement *pipeline)
{
    gint64 pos, len;
    if (gst_element_query_position (pipeline, GST_FORMAT_TIME, &pos)
      && gst_element_query_duration (pipeline, GST_FORMAT_TIME, &len)) {
      g_print ("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r",
           GST_TIME_ARGS (pos), GST_TIME_ARGS (len));
    }

    /* call me again */
    return TRUE;
}

void GstOps::pause_music()
{
    gst_element_set_state(GST_ELEMENT(data->pipeline), GST_STATE_PAUSED);
    std::cout << "Gstreamer pause" << std::endl;
}

void GstOps::resume_music()
{
    gst_element_set_state(GST_ELEMENT(data->pipeline), GST_STATE_PLAYING);
    std::cout << "Gstreamer resume" << std::endl;
}

gboolean GstOps::pause_resume_pipeline(CustomData *cust)
{
    if (cust->isPlaying) {
        gst_element_set_state(GST_ELEMENT(cust->pipeline), GST_STATE_PLAYING);
    }
    else {
        gst_element_set_state(GST_ELEMENT(cust->pipeline), GST_STATE_PAUSED);
    }

    return TRUE;
}

gboolean GstOps::volume_change(CustomData *cust)
{
    g_object_set(cust->volume, "volume", cust->curr_volume, NULL);
}

void GstOps::volume_up()
{
    data->curr_volume += 0.05;
    if (data->curr_volume > 1){
        data->curr_volume = 1;
    }
    g_object_set(data->volume, "volume", data->curr_volume, NULL);
}

void GstOps::volume_down()
{
    data->curr_volume -= 0.05;
    if (data->curr_volume < 0){
        data->curr_volume = 0;
    }
    g_object_set(data->volume, "volume", data->curr_volume, NULL);
}
