#include "Video.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
namespace rr
{

Video::Video() :videocb_(NULL), user_(NULL)
{

}

Video::~Video()
{

}

void Video::RegUser(VideoCB cb, void* user)
{
	videocb_ = cb;
	user_ = user;
}

void Video::GetVideo(const int& w, const int&h, const char* buff, int const& size)
{
	if (user_ == NULL || videocb_ == NULL || buff == NULL)
	{
		return;
	}
	videocb_(w, h, buff, user_);
}void Video::GetVideo(cv::Mat& img){
	{
		
	}}

}


