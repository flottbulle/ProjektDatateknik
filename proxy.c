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

	GstElement *pipeline, *source1, *source2, *sink1, *sink2, *jitter1, *jitter2;
	GstBus *bus;
	GstMessage *msg;
	GMainLoop *loop;
	gchar *camip, *camip2;
	camip="rtsp://192.168.2.130/h264.sdp";
	camip2="rtsp://192.168.2.131/h264.sdp";

	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	

	source1 = gst_element_factory_make("rtspsrc","src_1");
	source2 = gst_element_factory_make("rtspsrc","src_2");
	jitter1 = gst_element_factory_make("rtpjitterbuffer", "jitter_1");
	jitter2 = gst_element_factory_make("rtpjitterbuffer", "jitter_2");
	sink1 = gst_element_factory_make("udpsink", "udpsink1");
	sink2 = gst_element_factory_make("udpsink", "udpsink2");
	
	pipeline = gst_pipeline_new("the-pipeline");
	

	if (!pipeline || !source1 || !source2 || !sink1 || !sink2){
 		g_printerr ("One element could not be created. Exiting.\n");
		return -1;
	}

	g_object_set(G_OBJECT(source1), "location", camip, "latency", 5, "protocols", 0x00000007, NULL);
	g_object_set(G_OBJECT(source2), "location", camip2, "latency", 5, "protocols", 0x00000007, NULL);
	

	g_object_set(G_OBJECT(sink1), "host", "127.0.0.1", "port", 1234, NULL);
	g_object_set(G_OBJECT(sink2), "host", "127.0.0.1", "port", 1235, NULL);
	
	g_object_set(G_OBJECT(jitter1), "latency", 5, NULL);
	g_object_set(G_OBJECT(jitter2), "latency", 5, NULL);

	gst_bin_add_many (GST_BIN(pipeline), source1, source2, jitter1, jitter2, sink1, sink2, NULL);


	if (!(gst_element_link_many(jitter1, sink1, NULL)) || 
	!(gst_element_link_many(jitter2, sink2, NULL))){
		g_print ("Error linking elements");
  		return -1;
	}

	g_signal_connect (source1, "pad-added", G_CALLBACK (cb_new_pad), jitter1);	
	g_signal_connect (source2, "pad-added", G_CALLBACK (cb_new_pad), jitter2);	
	
	

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
	return 0;
}
