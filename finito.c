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

int main(int argc, char *argv[]) {

	GstElement *pipeline, *source, *decoder, *depay, 
	*source2, *videosink, *decoder2, *depay2, 
	*source3, *decoder3, *depay3, 
	*source4, *decoder4, *depay4, 
	*mixer, *jitter1, *jitter2, *jitter3, *jitter4, 
	*textoverlay1, *textoverlay2, *textoverlay3, *textoverlay4, 		*convert1, *filter, *filter2, *filter3, *filter4, 
	*videoscale, *videoscale2, *videoscale3, *videoscale4;

	GstPad *mixer_sink_pad, *mixer_sink_pad2, *filter_src_pad, *filter_src_pad2, *mixer_sink_pad3, *mixer_sink_pad4, *filter_src_pad3, *filter_src_pad4;

	GstBus *bus;
	GstMessage *msg;
	GMainLoop *loop;
	gchar *camip, *camip2;
	GstCaps *caps, *caps2, *caps3, *caps4;
	gint LATENCY = 25;
	gint JITTERLAT = 50;
	gint DECODESKIP= 2;
	gint vHeightFRONT = 750;
	gint vWidthFRONT = 950;
	gint vHeightBACK = 330;
	gint vWidthBACK = 550;
	gint vHeightLEFT = 800;
	gint vWidthLEFT = 550;
	gint vHeightRIGHT = 800;
	gint vWidthRIGHT = 550;
	gint FRONT_POS_X = 500;
	gint FRONT_POS_Y = 0;
	gint BACK_POS_X = 700;
	gint BACK_POS_Y = 750;
	gint LEFT_POS_X = 0;
	gint LEFT_POS_Y = 50;
	gint RIGHT_POS_X = 1380;
	gint RIGHT_POS_Y = 50;


	camip="rtsp://192.168.2.130/h264.sdp";
	camip2="rtsp://192.168.2.131/h264.sdp";
	gchar *name;
	gboolean *gtrue;
	gboolean *gfalse;
	gtrue =TRUE;
	gfalse = FALSE;
	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	/* Create Elements for pipeline	*/
	videosink = gst_element_factory_make("ximagesink", "sink");
	source = gst_element_factory_make("rtspsrc","src_1");
	decoder = gst_element_factory_make("avdec_h264", "h264decoder");	
	depay = gst_element_factory_make("rtph264depay", "depayload");	
	jitter1 = gst_element_factory_make("rtpjitterbuffer", "jitter1");	
	textoverlay1 = gst_element_factory_make("textoverlay", "text1");	
	convert1 = gst_element_factory_make("videoconvert", "conv1");	

	source2 = gst_element_factory_make("rtspsrc","src_2");
	decoder2 = gst_element_factory_make("avdec_h264", "h264decoder2");	
	depay2 = gst_element_factory_make("rtph264depay", "depayload2");	
	jitter2 = gst_element_factory_make("rtpjitterbuffer", "jitter2");
	textoverlay2 = gst_element_factory_make("textoverlay", "text2");			

	source3 = gst_element_factory_make("rtspsrc","src_3");
	decoder3 = gst_element_factory_make("avdec_h264", "jpgdecoder3");	
	depay3 = gst_element_factory_make("rtph264depay", "depayload3");	
	jitter3 = gst_element_factory_make("rtpjitterbuffer", "jitter3");
	textoverlay3 = gst_element_factory_make("textoverlay", "text3");		

	source4 = gst_element_factory_make("rtspsrc","src_4");
	decoder4 = gst_element_factory_make("avdec_h264", "h264decoder4");	
	depay4 = gst_element_factory_make("rtph264depay", "depayload4");	
	jitter4 = gst_element_factory_make("rtpjitterbuffer", "jitter4");
	textoverlay4 = gst_element_factory_make("textoverlay", "text4");		

	mixer = gst_element_factory_make("videomixer", "mixer");

	/* UPPER cameras size */
	videoscale = gst_element_factory_make("videoscale", "scale");
	caps = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidthFRONT, "height", G_TYPE_INT, vHeightFRONT, NULL);
	filter = gst_element_factory_make("capsfilter", "filter1");
	g_object_set(G_OBJECT(filter), "caps", caps, NULL); 
	
	/* LOWER camera size */
	videoscale4 = gst_element_factory_make("videoscale", "scale4");
	caps4 = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidthBACK, "height", G_TYPE_INT, vHeightBACK, NULL);
	filter4 = gst_element_factory_make("capsfilter", "filter4");
	
	/* LEFT camera size */
	videoscale2 = gst_element_factory_make("videoscale", "scale2");
	caps2 = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidthLEFT, "height", G_TYPE_INT, vHeightLEFT, NULL);
	filter2 = gst_element_factory_make("capsfilter", "filter2");
	g_object_set(G_OBJECT(filter2), "caps", caps2, NULL);
	
	/* RIGHT camera size */
	videoscale3 = gst_element_factory_make("videoscale", "scale3");
	caps3 = gst_caps_new_simple ("video/x-raw", "width", G_TYPE_INT, vWidthRIGHT, "height", G_TYPE_INT, vHeightRIGHT, NULL);
	filter3 = gst_element_factory_make("capsfilter", "filter3");
	g_object_set(G_OBJECT(filter3), "caps", caps3, NULL);

	
	g_object_set(G_OBJECT(filter4), "caps", caps4, NULL);
	g_object_set(G_OBJECT(videoscale), "add-borders", gfalse, NULL);
	g_object_set(G_OBJECT(videoscale2), "add-borders", gfalse, NULL);
	g_object_set(G_OBJECT(videoscale3), "add-borders", gfalse, NULL);
	g_object_set(G_OBJECT(videoscale4), "add-borders", gfalse, NULL);


	pipeline = gst_pipeline_new("the-pipeline");
	
	if (!videosink || !source || !decoder|| !depay || !jitter1 || !source2 || !decoder2 || !depay2 || !jitter2 || !source3 || !decoder3 || !depay3 || !jitter3 || !source4 || !decoder4 || !depay4 || !jitter4 || !textoverlay1 || !textoverlay2 || !textoverlay3 || !textoverlay4 || !convert1 || !videoscale || !videoscale2 || !videoscale3 || !videoscale4) {
	    g_printerr ("One element could not be created. Exiting.\n");
	    return -1;
	}
	g_object_set(G_OBJECT(source), "location", camip, "buffer-mode", 0, "latency", JITTERLAT, "protocols", 0x00000007, NULL);
	g_object_set(G_OBJECT(source2), "location", camip2, "buffer-mode", 0, "latency", JITTERLAT,  "protocols", 0x00000007, NULL);
	g_object_set(G_OBJECT(source3), "location", camip2, "buffer-mode", 0, "latency", JITTERLAT, "protocols", 0x00000007, NULL);
	g_object_set(G_OBJECT(source4), "location", camip, "buffer-mode", 0, "latency", JITTERLAT, "protocols", 0x00000007, NULL);
	g_object_set(G_OBJECT(decoder), "skip-frame", DECODESKIP, "lowres", 0, NULL);
	g_object_set(G_OBJECT(decoder2), "skip-frame", DECODESKIP, "lowres", 0, NULL);
	g_object_set(G_OBJECT(decoder3), "skip-frame", DECODESKIP, "lowres", 0, NULL);
	g_object_set(G_OBJECT(decoder4), "skip-frame", DECODESKIP, "lowres", 0, NULL);
	
	g_object_set(G_OBJECT(jitter1), "latency", LATENCY, NULL);
	g_object_set(G_OBJECT(jitter2), "latency", LATENCY, NULL);
	g_object_set(G_OBJECT(jitter3), "latency", LATENCY, NULL);
	g_object_set(G_OBJECT(jitter4), "latency", LATENCY, NULL);

	g_object_set(G_OBJECT(textoverlay1), "text", "Front Cam", "valignment", 2, NULL);
	g_object_set(G_OBJECT(textoverlay2), "text", "Left Cam", "valignment", 2, NULL);
	g_object_set(G_OBJECT(textoverlay3), "text", "Rigth Cam", "valignment", 2, NULL);
	g_object_set(G_OBJECT(textoverlay4), "text", "Back Cam", "valignment", 2, NULL);

	//g_object_set(G_OBJECT(videosink), "fps-update-interval", 500, NULL);
	g_object_set(G_OBJECT(videosink), "sync", gfalse, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source, jitter1, depay, decoder, textoverlay1, videoscale, filter, mixer, convert1, videosink, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source2, jitter2, depay2, decoder2, videoscale2, filter2, textoverlay2, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source3, jitter3, depay3, decoder3, videoscale3, filter3, textoverlay3,  NULL);
	gst_bin_add_many (GST_BIN(pipeline), source4, jitter4, depay4, decoder4, videoscale4, filter4, textoverlay4, NULL);
	
	/* LINK ELEMENTS */
	if (!(gst_element_link_many(jitter1, depay, decoder, textoverlay1, videoscale, filter, NULL)) || 
!(gst_element_link_many(jitter2, depay2, decoder2, textoverlay2, videoscale2, filter2, NULL)) || 
!(gst_element_link_many(jitter3, depay3, decoder3, textoverlay3, videoscale3, filter3, NULL)) || 
!(gst_element_link_many(jitter4, depay4, decoder4, textoverlay4, videoscale4, filter4, NULL))){
	g_print ("Error linking elements");
  	return -1;
	}
	
	
	if(!gst_element_link_many(mixer, videosink, NULL)){
		g_print("error linking mixer with sink");
		return -1;
	}
		else{
		g_print("\nmixer linked with videosink \n");
	}


	 /* listen for newly created pads */
 	g_signal_connect (source, "pad-added", G_CALLBACK (cb_new_pad), jitter1);	
	g_signal_connect (source2, "pad-added", G_CALLBACK (cb_new_pad), jitter2);	
	g_signal_connect (source3, "pad-added", G_CALLBACK (cb_new_pad), jitter3);	
	g_signal_connect (source4, "pad-added", G_CALLBACK (cb_new_pad), jitter4);	

	/*	Same as above but not as function	*/
	GstPadTemplate *mixer_sink_pad_template;	

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

	/* Positioning on the video vindow */
	g_object_set(G_OBJECT(mixer), "background", 1, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad2), "xpos", LEFT_POS_X, "ypos", LEFT_POS_Y, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad3), "xpos", RIGHT_POS_X, "ypos", RIGHT_POS_Y, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad), "xpos", FRONT_POS_X, "ypos", FRONT_POS_Y, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad4), "xpos", BACK_POS_X, "ypos", BACK_POS_Y, NULL);
	
	gst_object_unref(mixer_sink_pad);
	gst_object_unref(filter_src_pad);
	gst_object_unref(mixer_sink_pad2);
	gst_object_unref(filter_src_pad2);
	gst_object_unref(mixer_sink_pad3);
	gst_object_unref(filter_src_pad3);
	gst_object_unref(mixer_sink_pad4);
	gst_object_unref(filter_src_pad4);
	
	g_print("\nWe're at the end of the line here \n");
	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	GstQuery *q;
	q = gst_query_new_latency();
	if(gst_element_query (pipeline, q)){
		gboolean live;
		GstClockTime minlat, maxlat;
		gst_query_parse_latency (q, &live, &minlat, &maxlat);
		g_print("\nWe're at the end of the line here \n");

		g_print ("duration = %"GST_TIME_FORMAT, GST_TIME_ARGS (q));
		g_print("\nWe're at the end of the line here \n");
		gst_query_set_latency (q,
                       live,
                       minlat,
                       maxlat);
	}
	gst_query_unref (q); 

	
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	

	/* Wait until error or EOS */
	bus = gst_element_get_bus (pipeline);

	msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE , GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

	/* Free resources */
	if (msg != NULL){
	gst_message_unref (msg);
	gst_object_unref (bus);
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (pipeline);
	g_main_loop_unref (loop);
	}
	return 0;
}
 
