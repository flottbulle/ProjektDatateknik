#include <gst/gst.h>
  
int main(int argc, char *argv[]) {

	GstElement *pipeline, *source, *encoder, *videomixer, *videosink, *queue1, *queue2, *payload;
	GstBus *bus;
	GstMessage *msg;
	GstPadTemplate *mixer_src_pad_template;
	GstPad *mixer_video_pad, *sink_pad;
	GstCaps *caps;
	/* Initialize GStreamer */
	gst_init (&argc, &argv);
	
	
	
	videosink = gst_element_factory_make("udpsink", "sink");
	source = gst_element_factory_make("v4l2src","camsrc");
	queue1 = gst_element_factory_make("queue", "q1");
	queue2 = gst_element_factory_make("queue", "q2");
	videomixer = gst_element_factory_make("videomixer", "mixer");
	encoder = gst_element_factory_make("x264enc", "avdec");
	payload = gst_element_factory_make("rtph264pay", "pay");
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
	
	if (!videosink || !source || !queue1 || !queue2 || !encoder || !videomixer || !payload) {
	    g_printerr ("One element could not be created. Exiting.\n");
	    return -1;
	}
	g_object_set(videosink, "host", "127.0.0.1", "port", 1236, NULL);
	g_object_set(G_OBJECT(source), "caps", gst_caps_new_simple("video/x-raw",
					"width", G_TYPE_INT, 640,
					"height", G_TYPE_INT, 480, NULL), NULL);
	g_object_set(encoder, "speed-preset", 2, "tune", 0x00000004, NULL);

	/* ADD TO A BIN */
	gst_bin_add_many (GST_BIN(pipeline), source, queue1, encoder, queue2, payload, videosink, NULL);

	
	
	/* LINK ELEMENTS */
	if (gst_element_link_many(source, queue1, encoder, queue2, payload, videosink, NULL)!= TRUE){
	g_print ("Error linking elements");
	}

	
	
	/* 130.240.152.93 */
	gst_element_set_state (pipeline, GST_STATE_PLAYING);

	/* Wait until error or EOS */
	bus = gst_element_get_bus (pipeline);
	msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

	/* Release the request pads from the Tee, and unref them */
	//gst_element_release_request_pad (videomixer, mixer_video_pad);
	//gst_object_unref (mixer_video_pad);
	/* Free resources */
	if (msg != NULL)
	gst_message_unref (msg);
	gst_object_unref (bus);
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (pipeline);
	return 0;
}
 
