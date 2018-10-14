#include "Camera.h"
#include <stdio.h>
#include "../baseutils/RrConfig.h"
namespace rr
{

Camera::Camera() :face_detector_(NULL), factory_(NULL), rate_(25), video_w_(640), video_h_(480), video_flag_(false), ipc_id_(""), dev_id_("")
{
	factory_ = new FactoryAlgorithmfdc();
	if (factory_ == NULL)
	{
		printf("factory_ is NULL\n");
		return;
	}
	factory_->CreateFaceDetector(&face_detector_);
	if (face_detector_ == NULL)
	{
		printf("create face verify failed!!!\n");
		return;
	}
	RrConfig config;
	config.ReadConfig("config.ini");
	rate_ = config.ReadInt("RATE", "Rate", 25);
	if (rate_ > 0)
	{
		rate_ = 25 / rate_;
		printf("rate = %d", rate_);
	}
	video_w_ = config.ReadInt("Video", "Width", 640);
	video_h_ = config.ReadInt("Video", "Heigth", 480);
}

Camera::~Camera()
{
	if (face_detector_ && factory_)
	{
		factory_->DestroyFaceDetector(face_detector_);
		delete factory_;
		factory_ = NULL;
	}
}

void Camera::setDev(const std::string& devid, const std::string& ipcid)
{
	if (face_detector_)
	{
		face_detector_->setDev(devid, ipcid);
	}
}

void Camera::setVideoFlag(const std::string& flag, const std::string& ipcid, const std::string& devid)
{
	if (flag == "true")
	{

		video_flag_ = true;
		ipc_id_ = ipcid;
		dev_id_ = devid;
	}
	else
	{
		video_flag_ = false;
	}
}

bool Camera::getQuitState() const
{
	return false;
}

}