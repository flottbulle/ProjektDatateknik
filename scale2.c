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
/*
static void new_sink_request_pad (GstElement *mixer, GstElement *filter) {
	g_print("\n This method is now running! \n");
	GstPad *sinkpad, *srcpad;
        gchar *name;
	GstPadTemplate *mixer_sink_pad_template;
	
	mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	sinkpad = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        g_object_set(G_OBJECT(sinkpad), "xpos", 0, "ypos", 0, NULL);
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
	gst_object_unref (GST_OBJECT(srcpad));
}*/



int main(int argc, char *argv[]) {

	GstElement *pipeline, *source, *decoder, *depay, 
*source2, *videosink, *decoder2, *depay2, *videosink2, 
*source3, *decoder3, *depay3, *videosink3, 
*source4, *decoder4, *depay4, *videosink4, 
*mixer, *filter, *filter2, *filter3, *filter4, *convert, *videoscale, *videoscale2, *videoscale3, *videoscale4;

	GstPad *mixer_sink_pad, *mixer_sink_pad2, *filter_src_pad, *filter_src_pad2, *mixer_sink_pad3, *mixer_sink_pad4, *filter_src_pad3, *filter_src_pad4;
	GstBus *bus;
	GstMessage *msg;
	GMainLoop *loop;
	gchar *camip, *camip2;
	gint LATENCY = 50;
	gint DECODESKIP = 2;
	camip="rtsp://192.168.10.4/h264.sdp";
	camip2="rtsp://192.168.10.2/h264.sdp";
	GstCaps *caps, *caps2, *caps3, *caps4;
	gchar *name;
	gint vHeight = 360;
	gint vWidth = 640;
	gint vHeight2 = 640;
	gint vWidth2 = 360;
	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	/* Create Elements for pipeline	*/
	videosink = gst_element_factory_make("ximagesink", "sink");
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
	
	convert = gst_element_factory_make("videoconvert", "converter");
	mixer = gst_element_factory_make("videomixer", "mixer");



	videoscale = gst_element_factory_make("videoscale", "scale");
	caps = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidth, "height", G_TYPE_INT, vHeight, NULL);
	filter = gst_element_factory_make("capsfilter", "filter1");
	g_object_set(G_OBJECT(filter), "caps", caps, NULL); 
	
	videoscale2 = gst_element_factory_make("videoscale", "scale2");
	caps2 = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidth, "height", G_TYPE_INT, vHeight, NULL);
	filter2 = gst_element_factory_make("capsfilter", "filter2");
	g_object_set(G_OBJECT(filter2), "caps", caps2, NULL);
	
	videoscale3 = gst_element_factory_make("videoscale", "scale3");
	caps3 = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidth2, "height", G_TYPE_INT, vHeight2, NULL);
	filter3 = gst_element_factory_make("capsfilter", "filter3");
	g_object_set(G_OBJECT(filter3), "caps", caps3, NULL);

	videoscale4 = gst_element_factory_make("videoscale", "scale4");
	caps4 = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidth2, "height", G_TYPE_INT, vHeight2, NULL);
	filter4 = gst_element_factory_make("capsfilter", "filter4");
	g_object_set(G_OBJECT(filter4), "caps", caps4, NULL);

	pipeline = gst_pipeline_new("the-pipeline");
	
	if (!videosink || !source || !decoder|| !depay || !source2 || !decoder2 || !depay2 || !videosink2 || !source3 || !decoder3 || !depay3 || !videosink3 || !source4 || !decoder4 || !depay4 || !videosink4 || !filter || !filter2 || !filter3 || !filter4) {
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
	gst_bin_add_many (GST_BIN(pipeline), source, decoder, depay, videoscale, filter, mixer, convert, videosink, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source2, decoder2, depay2, videoscale2, filter2,  NULL);
	gst_bin_add_many (GST_BIN(pipeline), source3, decoder3, depay3, videoscale3, filter3, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source4, decoder4, depay4, videoscale4, filter4, NULL);
	
	/* LINK ELEMENTS */
	if (!(gst_element_link_many(depay, decoder, videoscale, filter, NULL)) || 
!(gst_element_link_many(depay2, decoder2, videoscale2, filter2, NULL)) || 
!(gst_element_link_many(depay3, decoder3, videoscale3, filter3, NULL)) || 
!(gst_element_link_many(depay4, decoder4, videoscale4, filter4, NULL))){
	g_print ("Error linking elements");
  	return -1;
	}
	
	
	if(!gst_element_link_many(mixer, convert, videosink, NULL)){
		g_print("error linking mixer with sink");
		return -1;
	}
		else{
		g_print("\nmixer linked with videosink \n");
	}


	 /* listen for newly created pads */
 	g_signal_connect (source, "pad-added", G_CALLBACK (cb_new_pad), depay);	
	g_signal_connect (source2, "pad-added", G_CALLBACK (cb_new_pad), depay2);	
	g_signal_connect (source3, "pad-added", G_CALLBACK (cb_new_pad), depay3);	
	g_signal_connect (source4, "pad-added", G_CALLBACK (cb_new_pad), depay4);	
	
	
	/* Function calls to create and link request pads */
	//new_sink_request_pad(mixer, filter);
	//new_sink_request_pad(mixer, filter1);



	/*	Same as above but not as function	*/

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
	
	filter_src_pad2 = gst_element_get_static_pad(filter2, "src");
	name = gst_pad_get_name (filter_src_pad2);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	
	if(gst_pad_link (filter_src_pad2, mixer_sink_pad2) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return -1;
	}

mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	mixer_sink_pad3 = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        
	name = gst_pad_get_name (mixer_sink_pad3);
	g_print ("A new request pad %s was created\n", name);
	g_free (name);
	
	filter_src_pad3 = gst_element_get_static_pad(filter3, "src");
	name = gst_pad_get_name (filter_src_pad3);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	
	if(gst_pad_link (filter_src_pad3, mixer_sink_pad3) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return -1;
	}
	mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	mixer_sink_pad4 = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        
	name = gst_pad_get_name (mixer_sink_pad4);
	g_print ("A new request pad %s was created\n", name);
	g_free (name);
	
	filter_src_pad4 = gst_element_get_static_pad(filter4, "src");
	name = gst_pad_get_name (filter_src_pad4);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	
	if(gst_pad_link (filter_src_pad4, mixer_sink_pad4) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return -1;
	}






	g_object_set(G_OBJECT(mixer), "background", 3, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad), "xpos", 0, "ypos", 0, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad2), "xpos", 640, "ypos", 0, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad3), "xpos", 0, "ypos", 360, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad4), "xpos", 640, "ypos", 360, NULL);
	
	
	
	
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
 
