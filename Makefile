CC=gcc
LIBS=`pkg-config gstreamer1.0 --libs`
CFLAGS=`pkg-config gstreamer1.0 --cflags`

all:
	$(CC) $(LIBS) $(CFLAGS) gstreamtest.c
