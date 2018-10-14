#include "IPCamera.h"
#include "../baseutils/RrConfig.h"
#include <stdio.h>

namespace rr
{

IPCamera::IPCamera() : rtsp_url_(""), rtsp_flag_(""), video_(NULL)
{
#if 0
	rtsp_flag_ = "/ !rtph264depay !h264parse ! mppvideodec !video/x-raw,format=(string)NV12 !v4l2video0convert output-io-mode=dmabuf capture-io-mode=dmabuf !video/x-raw,format=(string)BGR, width=(int)1280,height=(int)720 !appsink caps=video/x-raw,format=BGR name=sink sync=false";
	video_ = new GstVideo();
#endif
}

IPCamera::IPCamera(const char* rtsp) : rtsp_url_(rtsp), rtsp_flag_(""), video_(NULL)
{
#if 0
	rtsp_flag_ = "/ !rtph264depay !h264parse ! mppvideodec !video/x-raw,format=(string)NV12 !v4l2video0convert output-io-mode=dmabuf capture-io-mode=dmabuf !video/x-raw,format=(string)BGR, width=(int)1280,height=(int)720 !appsink caps=video/x-raw,format=BGR name=sink sync=false";
	video_ = new GstVideo();
	rtsp_url_ += rtsp_flag_;
#endif
}

IPCamera::~IPCamera()
{

}

void IPCamera::Open()
{
#if 0
	printf("rtsp=%s", rtsp_url_.c_str());
	if (video_ == NULL)
	{
		printf("open camera failed!!!!\n");
		return;
	}
	video_->RegUser(GetVideoCB, this);
	if (!video_->InitVideo(rtsp_url_.c_str()))
	{
		printf("open camera failed!!!!\n");
		return;
	}
	face_detector_->start();
	printf("detect start!\n");
#endif
}

void IPCamera::Close()
{
#if 0
	if (video_ == NULL)
	{
		printf("close camera failed!!!!\n");
		return;
	}
	face_detector_->stop();
	if (!video_->UnInitVideo())
	{
		printf("close camera failed!!!!\n");
		return;
	}
#endif
}

void IPCamera::Run()
{
	
}

void IPCamera::GetVideoCB(const int& w, const int&h, const char* buff, void* user)
{
#if 0
	IPCamera* camer = static_cast<IPCamera*>(user);
	if (camer == NULL)
	{
		printf("do not work!!!!\n");
		return;
	}
	camer->ConstructeMat(w, h, buff);
#endif	
}

void IPCamera::ConstructeMat(const int& w, const int&h, const char* buff)
{
#if 0
	static long long frame = 0;
	if ((frame % rate_) == 0)
	{
		cv::Mat src(w, h, CV_8UC3, (char *)buff, cv::Mat::AUTO_STEP);
		face_detector_->RecvRtImg(src.clone());
	}
	frame++;
#endif
}
}


