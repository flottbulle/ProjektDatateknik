#include <gst/gst.h>
  
int main(int argc, char *argv[]) {

	GstElement *pipeline, *source, *decoder, *convert, *source2, *videosink, *decoder2, *convert2, *videosink2, *source3, *decoder3, *convert3, *videosink3, *source4, *decoder4, *convert4, *videosink4, *queue, *queue2, *queue3, *queue4, *demux, *demux2, *demux3, *demux4;
	GstBus *bus;
	GstMessage *msg;
	gchar *camip;
	camip="http://192.168.10.4/mjpeg";
	gboolean GTRUE = TRUE;
	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	
	videosink = gst_element_factory_make("autovideosink", "sink");
	source = gst_element_factory_make("souphttpsrc","soupsrc");
	demux = gst_element_factory_make("avidemux","demux");
	decoder = gst_element_factory_make("jpegdec", "jpgdecoder");	
	convert = gst_element_factory_make("videoconvert", "converter");	
	queue = gst_element_factory_make("queue", "q1");

	videosink2 = gst_element_factory_make("autovideosink", "sink2");
	source2 = gst_element_factory_make("souphttpsrc","soupsrc2");
	demux2 = gst_element_factory_make("avidemux","demux2");
	decoder2 = gst_element_factory_make("jpegdec", "jpgdecoder2");	
	convert2 = gst_element_factory_make("videoconvert", "converter2");	
	queue2 = gst_element_factory_make("queue", "q2");
	
	videosink3 = gst_element_factory_make("autovideosink", "sink3");
	source3 = gst_element_factory_make("souphttpsrc","soupsrc3");
	demux3 = gst_element_factory_make("avidemux","demux3");
	decoder3 = gst_element_factory_make("jpegdec", "jpgdecoder3");	
	convert3 = gst_element_factory_make("videoconvert", "converter3");	
	queue3 = gst_element_factory_make("queue", "q3");

	videosink4 = gst_element_factory_make("autovideosink", "sink4");
	source4 = gst_element_factory_make("souphttpsrc","soupsrc4");
	demux4 = gst_element_factory_make("avidemux","demux4");
	decoder4 = gst_element_factory_make("jpegdec", "jpgdecoder4");	
	convert4 = gst_element_factory_make("videoconvert", "converter4");	
	queue4 = gst_element_factory_make("queue", "q4");	

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
	
	if (!videosink || !source || !decoder|| !convert || !demux || !source2 || !decoder2 || !convert2 || !videosink2 || !demux2 || !source3 || !decoder3 || !convert3 || !videosink3 || !demux3 || !source4 || !decoder4 || !convert4 || !videosink4 || !demux4) {
	    g_printerr ("One element could not be created. Exiting.\n");
	    return -1;
	}
	g_object_set(G_OBJECT(source), "location", camip, "is-live", GTRUE, NULL);
	g_object_set(G_OBJECT(source2), "location", camip, "is-live", GTRUE,  NULL);
	g_object_set(G_OBJECT(source3), "location", camip, "is-live", GTRUE,  NULL);
	g_object_set(G_OBJECT(source4), "location", camip, "is-live", GTRUE,  NULL);
	g_object_set(decoder, "max-errors", "-1", NULL);
	g_object_set(decoder2, "max-errors", "-1", NULL);
	g_object_set(decoder3, "max-errors", "-1", NULL);
	g_object_set(decoder4, "max-errors", "-1", NULL);
	//g_object_set(demux, "single-stream", GTRUE, NULL);
	/* ADD TO A BIN */
	gst_bin_add_many (GST_BIN(pipeline), source, demux, decoder, convert, queue, videosink, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source2, demux2, decoder2, convert2, queue2, videosink2, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source3, demux3, decoder3, convert3, queue3, videosink3, NULL);
	gst_bin_add_many (GST_BIN(pipeline), source4, demux4, decoder4, convert4, queue4, videosink4, NULL);
	
	/* LINK ELEMENTS */
	if (!(gst_element_link_many(source, decoder, convert, videosink, NULL)) || 
!(gst_element_link_many(source2, decoder2, convert2, videosink2, NULL)) || 
!(gst_element_link_many(source3, decoder3, convert3, videosink3, NULL)) || 
!(gst_element_link_many(source4, decoder4, convert4, videosink4, NULL))){
	g_print ("Error linking elements");
  	return -1;
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
 
