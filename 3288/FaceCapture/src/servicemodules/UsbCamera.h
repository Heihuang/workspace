#ifndef _USBCamera_H_
#define _USBCamera_H_
#include <pthread.h>
#include <stdio.h>
#include "v4l2uvc.h"
#include "SendVideo.h"
#include "Camera.h"#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
namespace rr
{
class UsbCamera : public Camera
{
public:
	UsbCamera();
	virtual ~UsbCamera();
	virtual void Open();
	virtual void Close();
	void setVideoFlag(const std::string& flag, const std::string& ipcid, const std::string& devid);
	static void* work(void* param);
	virtual void Run();
	void QuitProcess();
	virtual bool getQuitState()const;
private:
	int width_;
	int height_;
	int fps_;
	bool video_flag_;
	std::string filename_;
	std::string ipc_id_;	std::string dev_id_;
	struct vdIn *cap_;	cv::VideoCapture *cap_cv_;;
	SendVideo* sender_video_;
	bool thread_flag_;
	bool quit_process_;
	long time_out_;
	pthread_t id_;
};
}

#endif
