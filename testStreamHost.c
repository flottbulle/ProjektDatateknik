#include <gst/gst.h>
  
int main(int argc, char *argv[]) {

  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  
  /* Initialize GStreamer */
  gst_init (&argc, &argv);
  
// v4l2src ! queue ! 'video/x-raw-yuv,width=640,height=480' ! x264enc bitrate=500 speed-preset=superfast tune=zerolatency ! queue ! rtph264pay ! udpsink host=224.1.1.1 port=5000 auto-multicast=true
  /* Build the pipeline */
  pipeline = gst_parse_launch ("rtpbin, v4l2src device=/dev/video0 ! queue ! video/*,width=1920,height=1080 !  x264enc speed-preset=superfast tune=zerolatency ! queue ! rtph264pay ! udpsink host=127.0.0.1 port=1236", NULL);
  /* 130.240.152.93 */
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
