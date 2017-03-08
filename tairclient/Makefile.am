AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=tairclient
tairclient_SOURCES= tairmain.cpp tairwrapper.cpp tairclient.cpp tairthread.cpp log.cpp CStreamPipe.cpp CThread.cpp Osi.cpp
CPPFLAGS+= -I./ -I../3rd/libevent-2.0.22-stable/ -I../3rd/libevent-2.0.22-stable/include/ -I../3rd/tair/install/include/ -I../3rd/tair/tair/src/client/ -I../3rd/tair/tair/src/packets -I../3rd/tair/tair/src/common/ -I../3rd/tair/tair/src/ -I../3rd/tair/install/include/tbnet/ -I../3rd/tair/install/include/tbsys/ -I../3rd/cJSON-master/
tairclient_LDFLAGS= -lpthread -lm -lrt -lz
tairclient_LDADD= ../3rd/libevent-2.0.22-stable/.libs/libevent.a ../3rd/libevent-2.0.22-stable/.libs/libevent_pthreads.a  ../3rd/libevent-2.0.22-stable/.libs/libevent_core.a ../3rd/cJSON-master/libcjson.a ../3rd/tair/install/lib/libtairclientapi.a ../3rd/tair/install/lib/libtbnet.a ../3rd/tair/install/lib/libtbsys.a -lrt

