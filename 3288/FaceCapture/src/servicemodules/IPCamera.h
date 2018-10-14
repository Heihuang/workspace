#ifndef IPCAMERA_H_

#define IPCAMERA_H_

#include <string>

#include <pthread.h>

#include "Camera.h"

#include "Video.h"

//#include "GstVideo.h"

#include "cv.h"

namespace rr

{

class IPCamera : public Camera

{

public:

	IPCamera();

	IPCamera(const char* rtsp);

	virtual ~IPCamera();

	virtual void Open();

	virtual void Close();

	virtual void Run();

	static void* work(void* param);

	static void GetVideoCB (const int& w, const int&h, const char* buff, void* user);

	void ConstructeMat(const int& w, const int&h, const char* buff);

private:

	std::string rtsp_url_;

	std::string rtsp_flag_;

	Video* video_;

	bool thread_flag_;

	pthread_t id_;

	cv::Mat img_;

};

}

#endif

