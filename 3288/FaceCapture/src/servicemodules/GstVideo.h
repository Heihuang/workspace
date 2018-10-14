#ifndef GSTVIDEO_H_
#define GSTVIDEO_H_
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include "Video.h"

namespace rr
{
class GstVideo : public Video
{
public:
	GstVideo();
	virtual ~GstVideo();
	virtual bool InitVideo(const char* rtsp);
	virtual bool UnInitVideo();
	static GstFlowReturn GetgstVideo(GstAppSink *appsink, gpointer data);
private:
	GError *error_;
	GMainLoop *loop_;
	gchar *descr_;
	GstElement *pipeline_;
	GstElement *sink_;
	GstAppSinkCallbacks callbacks_;
	bool InitGst(const char* rtsp);
	bool UnInitGst();
};

}
#endif
