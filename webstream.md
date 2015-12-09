// Stream from webcam source ///

gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=640,height=480' !  x264enc pass=qual quantizer=20 tune=zerolatency ! rtph264pay ! udpsink host=127.0.0.1 port=1234



/// Client fetch webcam stream ////

gst-launch-1.0 udpsrc port=1236 ! "application/x-rtp, payload=127" ! rtph264depay ! avdec_h264 ! fpsdisplaysink sync=false text-overlay=false




//////// ALTERNATIVE WAY ////////////////
//Sender:

gst-launch-1.0 v4l2src !
 video/x-raw,width=640,height=480 !
 timeoverlay !
 tee name="local" !
 queue !
 autovideosink local. !
 queue ! jpegenc! rtpjpegpay !
 udpsink host=127.0.0.1 port= 5000

//Receiver:
gst-launch-1.0 udpsrc port=5000 !
 application/x-rtp,
 encoding-name=JPEG,payload=26 ! 
 rtpjpegdepay ! jpegdec ! autovideosink
 
 /// link:http://www.z25.org/static/_rd_/videostreaming_intro_plab/


