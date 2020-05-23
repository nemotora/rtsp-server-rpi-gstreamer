#!/bin/bash
cd $(dirname $0)

SRC=/home/pi/gst-rtsp-server-1.14.4
echo $SRC
cp $SRC/gst/rtsp-server/.libs/libgstrtspserver-1.0.so.0 .

gcc -I$SRC -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -pthread -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -O2 -o raspi-rtsp server.c $PWD/libgstrtspserver-1.0.so.0 -lgstreamer-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -pthread -Wl,-rpath -Wl,$PWD
