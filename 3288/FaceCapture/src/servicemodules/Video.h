#ifndef VIDEO_H_
#define VIDEO_H_#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
namespace rr
{
typedef void(*VideoCB)(const int& w, const int&h, const char* buff, void* user);
class Video
{
public:
	Video();
	virtual ~Video();
	virtual bool InitVideo(const char* rtsp) = 0;
	virtual bool UnInitVideo() = 0;
	void RegUser(VideoCB  cb, void* user);
	void GetVideo(const int& w, const int&h, const char* buff,int const& size);	void GetVideo(cv::Mat& img);
private:
	VideoCB videocb_;
	void *user_;
};
}
#endif
