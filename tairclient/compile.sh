#!/bin/bash

set -e


#cd ../common-libs/yasm-1.3.0
#./configure
#make 
#make install
#cd -

cd ../common-libs/zlib-1.2.5
./configure --static
make
cd -

cd ../common-libs/ffmpeg-2.1.1
./configure --disable-demuxers --disable-muxers --enable-demuxer=flv --disable-avdevice --disable-avfilter --disable-encoders --enable-encoder=mjpeg --enable-encoder=png --extra-ldflags=-L../zlib-1.2.5 --extra-cflags=-I../zlib-1.2.5
make -j4
cd -

cd ../common-libs/libevent-2.0.21-stable
./configure --enable-static
make
cd -

./autogen.sh
./configure 
make
