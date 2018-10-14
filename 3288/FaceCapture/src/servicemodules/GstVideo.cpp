#include "GstVideo.h"
#include <stdio.h>

namespace rr
{

GstVideo::GstVideo() :error_(NULL), loop_(NULL), descr_(NULL), pipeline_(NULL), sink_(NULL)
{

}

GstVideo::~GstVideo()
{

}

bool GstVideo::InitGst(const char* rtsp)
{
	try
	{
		gst_init(NULL, NULL);
		loop_ = g_main_loop_new(NULL, -1);
		descr_ = g_strdup(rtsp);
		pipeline_ = gst_parse_launch(descr_, &error_);

		if (error_ != NULL) 
		{
			g_print("could not construct pipeline: %s\n", error_->message);
			g_error_free(error_);
			return false;
		}
		/* get sink */
		sink_ = gst_bin_get_by_name(GST_BIN(pipeline_), "sink");

		gst_app_sink_set_emit_signals((GstAppSink*)sink_, true);
		gst_app_sink_set_drop((GstAppSink*)sink_, true);
		gst_app_sink_set_max_buffers((GstAppSink*)sink_, 1);
		callbacks_ = { NULL, NULL,GetgstVideo };
		//callbacks.new_sample = video;
		gst_app_sink_set_callbacks(GST_APP_SINK(sink_), &callbacks_, this, NULL);
		gst_element_set_state(GST_ELEMENT(pipeline_), GST_STATE_PLAYING);
	}
	catch (...)
	{
		printf("Init gst failed!!!!");
		return false;
	}
	return true;
}

bool GstVideo::UnInitGst()
{
	try
	{
		g_main_loop_quit(loop_);
		g_strdown(descr_);
		if (error_)
		{
			g_error_free(error_);
		}
		gst_deinit();
	}
	catch (...)
	{
		printf("UnInit gst failed!!!");
		return false;
	}
	return true;
}

GstFlowReturn GstVideo::GetgstVideo(GstAppSink *appsink, gpointer data)
{
	GstVideo   *self = static_cast<GstVideo*>(data);
	if (self == NULL)
	{
		return GST_FLOW_NOT_LINKED;
	}
	try
	{
		GstSample *sample = gst_app_sink_pull_sample(appsink);
		GstCaps *caps = gst_sample_get_caps(sample);
		GstBuffer *buffer = gst_sample_get_buffer(sample);
		const GstStructure *info = gst_sample_get_info(sample);

		// ---- Read frame and convert to opencv format ---------------
		GstMapInfo map;
		gst_buffer_map(buffer, &map, GST_MAP_READ);

		// convert gstreamer data to OpenCV Mat, you could actually
		// resolve height / width from caps...
		int height = 0;
		int width = 0;
		GstStructure *s = gst_caps_get_structure(caps, 0);
		gst_structure_get_int(s, "height", &height);
		gst_structure_get_int(s, "width", &width);
		if (map.data != NULL)
		{
			self->GetVideo(height,width, (const char*)map.data, map.size);
		}
		gst_buffer_unmap(buffer, &map);
		gst_sample_unref(sample);
	}
	catch (...)
	{
		printf("gst video callback failed!!!!");
		return GST_FLOW_NOT_LINKED;
	}
	return GST_FLOW_OK;
}

bool GstVideo::InitVideo(const char* rtsp)
{
	return InitGst(rtsp);
}

bool GstVideo::UnInitVideo()
{
	return UnInitGst();
}

}


