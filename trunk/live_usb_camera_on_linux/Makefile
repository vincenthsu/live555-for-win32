LIVE555= ../

all: zk_v4l2_x264_source video_source

LIB_SUFFIX=a
INCLUDES = -I$(LIVE555)/UsageEnvironment/include -I$(LIVE555)/groupsock/include \
	   -I$(LIVE555)/liveMedia/include -I$(LIVE555)/BasicUsageEnvironment/include
CXXFLAGS= -c -g3 -O0 -fPIC $(INCLUDES)
USAGE_ENVIRONMENT_DIR = $(LIVE555)/UsageEnvironment
USAGE_ENVIRONMENT_LIB = $(USAGE_ENVIRONMENT_DIR)/libUsageEnvironment.$(LIB_SUFFIX)
BASIC_USAGE_ENVIRONMENT_DIR = $(LIVE555)/BasicUsageEnvironment
BASIC_USAGE_ENVIRONMENT_LIB = $(BASIC_USAGE_ENVIRONMENT_DIR)/libBasicUsageEnvironment.$(LIB_SUFFIX)
LIVEMEDIA_DIR = $(LIVE555)/liveMedia
LIVEMEDIA_LIB = $(LIVEMEDIA_DIR)/libliveMedia.$(LIB_SUFFIX)
GROUPSOCK_DIR = $(LIVE555)/groupsock
GROUPSOCK_LIB = $(GROUPSOCK_DIR)/libgroupsock.$(LIB_SUFFIX)
LOCAL_LIBS =	$(LIVEMEDIA_LIB) $(GROUPSOCK_LIB) \
		$(BASIC_USAGE_ENVIRONMENT_LIB) $(USAGE_ENVIRONMENT_LIB)

zk_v4l2_x264_source: zk_v4l2_x264_source.o capture.o vcompress.o $(LOCAL_LIBS)
	$(CXX) -o $@ $^ -fPIC  -L/usr/local/lib -lavcodec -lswscale -lavutil -lx264 -g3 -O0 -lpthread

video_source: server.o sender.o capture.o vcompress.o
	$(CXX) -o $@ $^ -fPIC -lavcodec -lswscale -lavutil  -lx264 -lpthread 

cpp.o:
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -f *.o
	rm -f zk_v4l2_x264_source
	rm -f video_source
