 gst-launch-1.0 videomixer name=mix ! x264enc pass=qual quantizer=20 tune=zerolatency ! rtph264pay  ! udpsink host=127.0.0.1 port=1234 \
   v4l2src device=/dev/video0 ! video/x-raw, width=640, height=480 ! videobox border-alpha=0 top=0 left=0 ! mix. \
   v4l2src device=/dev/video0 ! video/x-raw, width=640, height=480 ! videobox border-alpha=0 top=0 left=-640 ! mix. \
   v4l2src device=/dev/video0 ! video/x-raw, width=640, height=480 ! videobox border-alpha=0 top=-640 left=0 ! mix. \
   v4l2src device=/dev/video0 ! video/x-raw, width=640, height=480 ! videobox border-alpha=0 top=-640 left=-640 ! mix. \
   videotestsrc pattern=3 ! video/x-raw, framerate=5/1, width=1920, height=1080 ! mix.
