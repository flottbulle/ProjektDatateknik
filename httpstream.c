#include <gst/gst.h>
  
int main(int argc, char *argv[]) {

	GstElement *pipeline, *source, *decoder, *convert, *queue, *encoder, *decoder2, *depay, *videosink;
	GstBus *bus;
	GstMessage *msg;
	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	
	videosink = gst_element_factory_make("ximagesink", "sink");
	source = gst_element_factory_make("souphttpsrc","soupsrc");
	queue = gst_element_factory_make("queue", "q1");
	decoder = gst_element_factory_make("jpegdec", "jpgdecoder");	
	convert = gst_element_factory_make("videoconvert", "converter");	

	encoder = gst_element_factory_make("x264enc", "h264encoder");
	decoder2 = gst_element_factory_make("avdec_h264", "h264decoder");	
	depay = gst_element_factory_make("rtph264depay", "depay");	
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
	
	if (!videosink || !source || !decoder || !queue || !convert || !encoder || !decoder2 || !depay) {
	    g_printerr ("One element could not be created. Exiting.\n");
	    return -1;
	}
	g_object_set(G_OBJECT(source), "location", "http://192.168.10.4/mjpeg" , NULL);
	g_object_set(encoder, "speed-preset", 2, "tune", 0x00000004, NULL);

	/* ADD TO A BIN */
	gst_bin_add_many (GST_BIN(pipeline), source, decoder, convert, encoder, decoder2, depay, videosink, NULL);

	
	
	/* LINK ELEMENTS */
	if (gst_element_link_many(source, decoder, convert, encoder, decoder2, depay, videosink, NULL)!= TRUE){
	g_print ("Error linking elements");
	}

	
	gst_element_set_state (pipeline, GST_STATE_PLAYING);

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
 
