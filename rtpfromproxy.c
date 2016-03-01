#include <gst/gst.h>


int main(int argc, char *argv[]) {

	GstElement *pipeline, *source1, *decoder, *depay, 
*source2, *videosink, *decoder2, *depay2,
*mixer, *q1, *jitter1, *jitter2, *textoverlay1, *textoverlay2, *convert1, *convert2;
	GstCaps *caps;
	GstPad *mixer_sink_pad, *mixer_sink_pad2, *filter_src_pad, *filter_src_pad2;
	GstBus *bus;
	GstMessage *msg;
	GMainLoop *loop;
	gchar *name;
	gint LATENCY = 100;
	gint DECODESKIP= 2;
	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	/* Create Elements for pipeline	*/
	videosink = gst_element_factory_make("ximagesink", "sink");
	source1 = gst_element_factory_make("udpsrc","src_1");
	decoder = gst_element_factory_make("avdec_h264", "h264decoder");	
	depay = gst_element_factory_make("rtph264depay", "depayload");	
	jitter1 = gst_element_factory_make("rtpjitterbuffer", "jitter1");	
	textoverlay1 = gst_element_factory_make("textoverlay", "text1");	
	convert1 = gst_element_factory_make("videoconvert", "conv1");	

	//videosink2 = gst_element_factory_make("autovideosink", "sink2");
	source2 = gst_element_factory_make("udpsrc","src_2");
	decoder2 = gst_element_factory_make("avdec_h264", "h264decoder2");	
	depay2 = gst_element_factory_make("rtph264depay", "depayload2");	
	jitter2 = gst_element_factory_make("rtpjitterbuffer", "jitter2");
	textoverlay2 = gst_element_factory_make("textoverlay", "text2");		
	convert2 = gst_element_factory_make("videoconvert", "conv2");	

	q1 = gst_element_factory_make("queue", "queueone");	
	//textert = gst_element_factory_make("textoverlay", "texterter");
	mixer = gst_element_factory_make("videomixer", "mixer");


	pipeline = gst_pipeline_new("the-pipeline");
	
	if (!videosink || !source1 || !decoder|| !depay || !jitter1 || !source2 || !decoder2 || !depay2 || !jitter2|| !textoverlay1 || !textoverlay2 || !convert1 || !convert2 || !q1) {
	    g_printerr ("One element could not be created. Exiting.\n");
	    return -1;
	}
	
	/* Set source port */
	g_object_set(G_OBJECT(source1), "port", 1234, NULL);
	g_object_set(G_OBJECT(source2), "port", 1235, NULL);
	
	g_object_set(G_OBJECT(decoder), "skip-frame", DECODESKIP, "lowres", 2, NULL);
	g_object_set(G_OBJECT(decoder2), "skip-frame", DECODESKIP, "lowres", 2, NULL);
	
	g_object_set(G_OBJECT(jitter1), "latency", LATENCY, NULL);
	g_object_set(G_OBJECT(jitter2), "latency", LATENCY, NULL);
	
	g_object_set(G_OBJECT(textoverlay1), "text", "Front Cam", "valignment", 2, NULL);
	g_object_set(G_OBJECT(textoverlay2), "text", "Left Cam", "valignment", 2, NULL);
	
	
	//g_object_set(demux, "single-stream", GTRUE, NULL);

	caps = gst_caps_new_simple("application/x-rtp",
	   "clock-rate", G_TYPE_INT, 90000,
	   NULL);
	g_object_set(source1, "caps", caps, NULL);
	g_object_set(source2, "caps", caps, NULL);
	gst_caps_unref(caps);
	
	/* ADD TO A BIN */
	gst_bin_add_many (GST_BIN(pipeline), source1, jitter1, depay, decoder, textoverlay1, convert1, mixer, q1, videosink, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source2, jitter2, depay2, decoder2, textoverlay2, convert2, NULL);

	
	/* LINK ELEMENTS */
	if (!(gst_element_link_many(source1, jitter1, depay, decoder, textoverlay1, convert1, NULL)) || 
!(gst_element_link_many(source2, jitter2, depay2, decoder2, textoverlay2, convert2, NULL))){
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


	/*	Same as above but not as function	*/

	GstPadTemplate *mixer_sink_pad_template;	

	mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	mixer_sink_pad2 = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        
	name = gst_pad_get_name (mixer_sink_pad2);
	g_print ("A new request pad %s was created\n", name);
	g_free (name);
	
	filter_src_pad2 = gst_element_get_static_pad(convert2, "src");
	name = gst_pad_get_name (filter_src_pad2);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	
	if(gst_pad_link (filter_src_pad2, mixer_sink_pad2) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return -1;
	}
	






	
	mixer_sink_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS    (mixer), "sink_%u");
	mixer_sink_pad = gst_element_request_pad (mixer, mixer_sink_pad_template, NULL, NULL);
        
	name = gst_pad_get_name (mixer_sink_pad);
	g_print ("A new request pad %s was created\n", name);
	g_free (name);
	
	filter_src_pad = gst_element_get_static_pad(convert1, "src");
	name = gst_pad_get_name (filter_src_pad);
	g_print ("A new static source pad %s was created\n", name);
	g_free (name);
	
	if(gst_pad_link (filter_src_pad, mixer_sink_pad) != GST_PAD_LINK_OK){
		g_print("link error videomixer");
		return -1;
	}
	




	g_object_set(G_OBJECT(mixer), "background", 1, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad2), "xpos", 0, "ypos", 0, NULL);
	g_object_set(G_OBJECT(mixer_sink_pad), "xpos", 600, "ypos", 0, NULL);



	gst_object_unref(mixer_sink_pad);
	gst_object_unref(filter_src_pad);
	gst_object_unref(mixer_sink_pad2);
	gst_object_unref(filter_src_pad2);
	
 	
	gst_element_set_state (pipeline, GST_STATE_PLAYING);


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
 
