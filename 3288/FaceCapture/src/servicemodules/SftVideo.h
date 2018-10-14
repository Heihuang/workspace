#ifndef SFTVIDEO_H_
#define SFTVIDEO_H_
#include "Video.h"
namespace rr
{
	class SftVideo : public Video
	{
	public:
		SftVideo();
		virtual ~SftVideo();
		virtual bool InitVideo(const char* rtsp);
		virtual bool UnInitVideo();
	};
}
#endif
