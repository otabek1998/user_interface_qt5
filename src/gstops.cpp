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
    data->loop = g_main_loop_new(NULL, FALSE);
    const char* convert_to_gchar = location.c_str();
    //GstBus *bus;
    //GstMessage *msg;
    GstStateChangeReturn ret;

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
    g_object_set(data->sink, "device", "hw:1", NULL);
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


    bus = gst_pipeline_get_bus(GST_PIPELINE(data->pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, data);
    gst_object_unref(bus);
    gst_element_set_state(GST_ELEMENT (data->pipeline), GST_STATE_PLAYING);
    data->isPlaying = true;

    //g_timeout_add (200, (GSourceFunc) cb_print_position, data->pipeline);
    //g_timeout_add(100, (GSourceFunc) pause_resume_pipeline, data);
    //g_timeout_add(100, (GSourceFunc) volume_change, data);


    g_main_loop_run(data->loop);

}


gboolean GstOps::bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
    CustomData *cust = (CustomData *) data;
    //GMainLoop *loop = (GMainLoop *) data;

    GError *err;
    gchar *debug_info;
    GstTagList *tags = NULL;

    switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS: {
      g_print ("End of stream\n");
      emit cust->onStreamEnded();
      //g_main_loop_quit (cust->loop);
      break;
    }

    case GST_MESSAGE_ERROR:{
      gst_message_parse_error (msg, &err, &debug_info);
      g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
      g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error (&err);
      g_free (debug_info);

      g_main_loop_quit (cust->loop);
      break;
    }

    case GST_MESSAGE_STATE_CHANGED: {
      /* We are only interested in state-changed messages from the pipeline */
      //if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->pipeline)) {
          GstState old_state, new_state, pending_state;
          gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
          //g_print ("Pipeline state changed from %s to %s:\n",
          //gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
        //}
       break;
    }

    case GST_MESSAGE_TAG:{
      gst_message_parse_tag (msg, &tags);
      gst_tag_list_foreach (tags, print_one_tag, cust);
      g_print("\n");
      gst_tag_list_unref(tags);
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

void GstOps::print_one_tag(const GstTagList * list, const gchar * tag, gpointer user_data)
{
    int i, num;
    CustomData *cust = (CustomData *) user_data;

    num = gst_tag_list_get_tag_size (list, tag);
    for (i = 0; i < num; ++i) {
        const GValue *val;
        std::string temp;

    val = gst_tag_list_get_value_index (list, tag, i);
    if (G_VALUE_HOLDS_STRING (val)) {
        if (strncmp(tag, GST_TAG_TITLE, 5) == 0){
            temp = g_value_get_string(val);
            cust->setSongName(temp);
            //cust->song_name = g_value_get_string(val);
            std::cout << "TITLE is found and value is " << g_value_get_string(val) << std::endl;
        }
        if (strncmp(tag, GST_TAG_ARTIST, 6) == 0){
            temp = g_value_get_string(val);
            cust->setArtist(temp);
            //cust->artist = g_value_get_string(val);
            std::cout << "ARTIST is found and value is " << cust->artist << std::endl;
        }
        std::cout << tag << std::endl;
        g_print ("1.\t%20s : %s\n", tag, g_value_get_string (val));
    }
    else if (GST_VALUE_HOLDS_SAMPLE(val)) {
        if (strncmp(tag, GST_TAG_IMAGE, 5) == 0){
            GstSample *sample = NULL;
            if (gst_tag_list_get_sample_index(list, tag, i, &sample))
            {
                GstBuffer *img = gst_sample_get_buffer(sample);
                if (img) {
                    GstMapInfo mapInfo;
                    (void)gst_buffer_map(img, &mapInfo, GST_MAP_READ);
                    std::cout << "Parsing Album Cover length is " << mapInfo.size << std::endl;
                    //memcpy(album_art, mapInfo.data, mapInfo.size); // transfered to setter function to implement signal
                    cust->setAlbumArt(mapInfo);
                    gst_buffer_unmap(img, &mapInfo);
                }
            }
        }
    }
    else if (GST_VALUE_HOLDS_BUFFER (val)) {
      GstBuffer *buf = gst_value_get_buffer (val);
      guint buffer_size = gst_buffer_get_size (buf);

      g_print ("\t%20s : buffer of size %u\n", tag, buffer_size);
    } /*else if (GST_VALUE_HOLDS_DATE_TIME (val)) {
      GstDateTime *dt = g_value_get_boxed (val);
      gchar *dt_str = gst_date_time_to_iso8601_string (dt);

      g_print ("\t%20s : %s\n", tag, dt_str);
      g_free (dt_str);
    }*/ else {
      g_print ("\t%20s : tag of type '%s'\n", tag, G_VALUE_TYPE_NAME (val));
    }
  }
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

void GstOps::changeMusic(std::string location)
{
    const char* conv = location.c_str();
    gst_element_set_state(GST_ELEMENT(data->pipeline), GST_STATE_NULL);
    g_object_set(data->source, "location", conv, NULL);
    gst_element_set_state(GST_ELEMENT(data->pipeline), GST_STATE_PLAYING);
}
