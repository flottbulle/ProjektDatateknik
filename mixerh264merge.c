#include <gst/gst.h>
static void
cb_new_pad (GstElement *element,
	    GstPad     *pad,
	    gpointer    data)
{
	GstPad *sinkpad;
	GstElement *depayload = (GstElement *) data;

	/* We can now link this pad with the rtp depayload sink pad */
	g_print ("Dynamic pad created, linking source/depayload\n");

	sinkpad = gst_element_get_static_pad (depayload, "sink");

	gst_pad_link (pad, sinkpad);

	gst_object_unref (sinkpad);
}

static void new_sink_request_pad (GstElement *mixer, GstElement *filter, gchar *str) {
	g_print("\n This method is now running! \n");
	GstPad *sinkpad, *srcpad;
        gchar *name;
	GstPadTemplate *mixer_sink_pad_template;
	
	mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	sinkpad = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        
	name = gst_pad_get_name (sinkpad);
	g_print ("A new request pad %s was created\n", name);
	g_free (name);
	
	srcpad = gst_element_get_static_pad(filter, "src");
	name = gst_pad_get_name (srcpad);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	

	if(gst_pad_link (srcpad, sinkpad) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return;
	}

       	gst_object_unref (GST_OBJECT(sinkpad));
}



int main(int argc, char *argv[]) {

	GstElement *pipeline, *source, *decoder, *depay, 
*source2, *videosink, *decoder2, *depay2, *videosink2, 
*source3, *decoder3, *depay3, *videosink3, 
*source4, *decoder4, *depay4, *videosink4, 
*mixer, *filter, *filter1;

	GstPad *mixer_sink_pad, *mixer_sink_pad2, *src_pad, *filter_src_pad, *filter_src_pad2;
	GstBus *bus;
	GstMessage *msg;
	GMainLoop *loop;
	gchar *camip, *camip2;
	gint LATENCY = 40;
	gint DECODESKIP = 1;
	camip="rtsp://192.168.10.4/h264.sdp";
	camip2="rtsp://192.168.10.2/h264.sdp";
	GstCaps *caps1, *caps_bg;
	gchar *name;
	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	/* Create Elements for pipeline	*/
	videosink = gst_element_factory_make("autovideosink", "sink");
	source = gst_element_factory_make("rtspsrc","src_1");
	decoder = gst_element_factory_make("avdec_h264", "h264decoder");	
	depay = gst_element_factory_make("rtph264depay", "depayload");	
	
	videosink2 = gst_element_factory_make("autovideosink", "sink2");
	source2 = gst_element_factory_make("rtspsrc","src_2");
	decoder2 = gst_element_factory_make("avdec_h264", "h264decoder2");	
	depay2 = gst_element_factory_make("rtph264depay", "depayload2");	
	
	videosink3 = gst_element_factory_make("autovideosink", "sink3");
	source3 = gst_element_factory_make("rtspsrc","src_3");
	decoder3 = gst_element_factory_make("avdec_h264", "jpgdecoder3");	
	depay3 = gst_element_factory_make("rtph264depay", "depayload3");	
	
	videosink4 = gst_element_factory_make("autovideosink", "sink4");
	source4 = gst_element_factory_make("rtspsrc","src_4");
	decoder4 = gst_element_factory_make("avdec_h264", "h264decoder4");	
	depay4 = gst_element_factory_make("rtph264depay", "depayload4");	
	
	mixer = gst_element_factory_make("videomixer", "mixer");


	caps_bg = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, 1000, "height", G_TYPE_INT, 1000, "framerate", GST_TYPE_FRACTION, 25, 1, NULL);
	filter = gst_element_factory_make("capsfilter", "filter-bg");
	g_object_set(G_OBJECT(filter), "caps", caps_bg, NULL);
	
	
	caps1 = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, 400, "height", G_TYPE_INT, 400, "framerate", GST_TYPE_FRACTION, 25, 1, NULL);
	filter1 = gst_element_factory_make("capsfilter", "filter1");
	g_object_set(G_OBJECT(filter1), "caps", caps1, NULL);
	

	pipeline = gst_pipeline_new("the-pipeline");

	/*
speed-preset        : Preset name for speed/quality tradeoff options (can affect decode compatibility - impose restrictions separately for your target decoder)
                        flags: readable, writable
                        Enum "GstX264EncPreset" Default: 6, "medium"
                           (0): None             - No preset
                           (1): ultrafast        - ultrafast
                           (2): superfast        - superfast
                           (3): veryfast         - veryfast
                           (4): faster           - faster
                           (5): fast             - fast
                           (6): medium           - medium
                           (7): slow             - slow
                           (8): slower           - slower
                           (9): veryslow         - veryslow
                           (10): placebo          - placebo
	*/
	
	if (!videosink || !source || !decoder|| !depay || !source2 || !decoder2 || !depay2 || !videosink2 || !source3 || !decoder3 || !depay3 || !videosink3 || !source4 || !decoder4 || !depay4 || !videosink4) {
	    g_printerr ("One element could not be created. Exiting.\n");
	    return -1;
	}
	g_object_set(G_OBJECT(source), "location", camip, "latency", LATENCY, "protocols", 0x00000001, NULL);
	g_object_set(G_OBJECT(source2), "location", camip, "latency", LATENCY,  "protocols", 0x00000001,  NULL);
	g_object_set(G_OBJECT(source3), "location", camip2, "latency", LATENCY, "protocols", 0x00000001,   NULL);
	g_object_set(G_OBJECT(source4), "location", camip2, "latency", LATENCY, "protocols", 0x00000001,   NULL);
	g_object_set(decoder, "skip-frame", DECODESKIP, NULL);
	g_object_set(decoder2, "skip-frame", DECODESKIP, NULL);
	g_object_set(decoder3, "skip-frame", DECODESKIP, NULL);
	g_object_set(decoder4, "skip-frame", DECODESKIP, NULL);
	//g_object_set(demux, "single-stream", GTRUE, NULL);
/*
	caps = gst_caps_new_simple ("video/x-raw",
	   "width", G_TYPE_INT, 1920,
	   "height", G_TYPE_INT, 1080,
	   NULL);*/
	/* ADD TO A BIN */
	gst_bin_add_many (GST_BIN(pipeline), source, decoder, depay, videosink, filter, filter1, mixer, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source2, decoder2, depay2, videosink2, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source3, decoder3, depay3, videosink3, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source4, decoder4, depay4, videosink4, NULL);
	
	/* LINK ELEMENTS */
	if (!(gst_element_link_many(depay, decoder, filter, NULL)) || 
!(gst_element_link_many(depay2, decoder2, filter1, NULL)) || 
!(gst_element_link_many(depay3, decoder3, NULL)) || 
!(gst_element_link_many(depay4, decoder4, NULL))){
	g_print ("Error linking elements");
  	return -1;
	}
	
	 /* listen for newly created pads */
 	g_signal_connect (source, "pad-added", G_CALLBACK (cb_new_pad), depay);	
	g_signal_connect (source2, "pad-added", G_CALLBACK (cb_new_pad), depay2);	
	g_signal_connect (source3, "pad-added", G_CALLBACK (cb_new_pad), depay3);	
	g_signal_connect (source4, "pad-added", G_CALLBACK (cb_new_pad), depay4);	
	
	
	/* Function calls to create and link request pads */
	new_sink_request_pad(mixer, filter, "sink_0");
	new_sink_request_pad(mixer, filter1, "sink_1");



	/*	Same as above but not as function	*/
/*
	GstPadTemplate *mixer_sink_pad_template;
	mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	mixer_sink_pad = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        
	name = gst_pad_get_name (mixer_sink_pad);
	g_print ("A new request pad %s was created\n", name);
	g_free (name);
	
	filter_src_pad = gst_element_get_static_pad(filter, "src");
	name = gst_pad_get_name (filter_src_pad);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	
	if(gst_pad_link (filter_src_pad, mixer_sink_pad) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return -1;
	}
	mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	mixer_sink_pad2 = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        
	name = gst_pad_get_name (mixer_sink_pad2);
	g_print ("A new request pad %s was created\n", name);
	g_free (name);
	
	filter_src_pad2 = gst_element_get_static_pad(filter1, "src");
	name = gst_pad_get_name (filter_src_pad2);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	
	if(gst_pad_link (filter_src_pad2, mixer_sink_pad2) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return -1;
	}
*/
	
	if(!gst_element_link(mixer, videosink)){
	g_print("error linking mixer with sink");
	return -1;
	}
	else{
	g_print("\nmixer linked with videosink \n");
	}

	

	
	g_print("\nWe're at the end of the line here \n");
	gst_element_set_state (pipeline, GST_STATE_PLAYING);
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	/* Wait until error or EOS */
	bus = gst_element_get_bus (pipeline);
	msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

	/* Free resources */
	if (msg != NULL)
	gst_message_unref (msg);
	gst_object_unref (bus);
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (pipeline);
	g_main_loop_unref (loop);
	return 0;
}
 
