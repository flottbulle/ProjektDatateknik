// Stream from webcam source ///

gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=640,height=480' !  x264enc pass=qual quantizer=20 tune=zerolatency ! rtph264pay ! udpsink host=127.0.0.1 port=1234



/// Client fetch webcam stream ////

gst-launch-1.0 udpsrc port=1234 ! "application/x-rtp, payload=127" ! rtph264depay ! avdec_h264 ! fpsdisplaysink sync=false text-overlay=false

