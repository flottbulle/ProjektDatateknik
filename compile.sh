#!/bin/bash
libtool --mode=link gcc -Wall startstream.c -o startstream $(pkg-config --cflags --libs gstreamer-1.0)
libtool --mode=link gcc -Wall getstream.c -o getstream $(pkg-config --cflags --libs gstreamer-1.0)
libtool --mode=link gcc -Wall streamOneCam.c -o streamOneCam $(pkg-config --cflags --libs gstreamer-1.0)
