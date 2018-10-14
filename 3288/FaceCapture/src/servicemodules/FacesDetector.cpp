#include "FacesDetector.h"
#include "../baseutils/RrConfig.h"
#include <stdio.h>
namespace rr
{
FaceDetector::FaceDetector() :list_size_(25), thread_flag_(false), fullview_sender_(NULL), rtface_sender_(NULL), faceupdate_sender_(NULL), decode_flag_(false), talk_id_("")
{

	RrConfig config;
	config.ReadConfig("config.ini");
	list_size_ = config.ReadInt("IMGLIST", "ImgList", 15);
	talk_id_ = config.ReadString("TALKID", "TalkId", "1");
	if (list_size_ < 5)
	{

		return;

	}
	pthread_mutex_init(&mutex_img_, NULL);
	pthread_mutex_init(&mutex_frame_, NULL);
	sem_init(&over_, 0, 1);

	//fullview_sender_ = new SendFullView();
	rtface_sender_ = new SendRtFace();
	faceupdate_sender_ = new SendFaceUpdate();
}

FaceDetector::~FaceDetector()
{
	pthread_mutex_destroy(&mutex_img_);
	pthread_mutex_destroy(&mutex_frame_);
	sem_destroy(&over_);
	/*if (fullview_sender_)

	{

		delete fullview_sender_;

		fullview_sender_ = NULL;

	}*/

	if (rtface_sender_)
	{
		delete rtface_sender_;
		rtface_sender_ = NULL;
	}
	if (faceupdate_sender_)
	{
		delete faceupdate_sender_;
		faceupdate_sender_ = NULL;

	}
	for (std::vector<Frame*>::iterator it = frame_set_.begin(); it != frame_set_.end(); ++it)
	{
		(*it)->Clear();
		delete *it;
		*it = NULL;
	}
	frame_set_.clear();
}

void FaceDetector::RecvRtImg(const cv::Mat& img)
{

	pthread_mutex_lock(&mutex_img_);
	if (img_list_.size() > list_size_)
	{
		img_list_.erase(img_list_.begin());
		img_list_.push_back(img);
	}
	else
	{
		img_list_.push_back(img);
	}
	pthread_mutex_unlock(&mutex_img_);
}

void FaceDetector::RecvRtImg(unsigned char* buff, const long& size)
{
	if (buff == NULL)
	{
		return;
	}
	Frame *fm = new Frame();
	if (fm)
	{
		//OutputLog_Info("tsw");
		fm->CreateFrame(buff, size);
		pthread_mutex_lock(&mutex_frame_);
		if (frame_set_.size() > list_size_)
		{
			frame_set_.clear();
		}
		frame_set_.push_back(fm);
		pthread_mutex_unlock(&mutex_frame_);
	}
	
}

void FaceDetector::CopyImg(std::vector<cv::Mat>& img_list)
{
	pthread_mutex_lock(&mutex_img_);
	img_list = img_list_;
	img_list_.clear();
	pthread_mutex_unlock(&mutex_img_);

}

void FaceDetector::start()
{
	thread_flag_ = true;
	decode_flag_ = true;
	pthread_create(&id_detect_, NULL, work, this);
	pthread_create(&id_decode_, NULL, supwork, this);
	printf("start detect thread\n");
}



void FaceDetector::stop()
{
	thread_flag_ = false;
	decode_flag_ = false;
	pthread_join(id_detect_, NULL);
	pthread_join(id_decode_, NULL);
	sem_post(&over_);

}



void FaceDetector::wait()
{
	sem_wait(&over_);
}



void* FaceDetector::work(void* param)
{
	FaceDetector* self = static_cast<FaceDetector*>(param);
	if (self)
	{
		//set police 
		//pthread_attr_t attr;
		//struct sched_param sched;
		//int rs = pthread_attr_init(&attr);
		//assert(rs == 0);
		//rs = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		//assert(rs == 0);
		self->Run();
		//rs = pthread_attr_destroy(&attr);
		//assert(rs == 0);
	}
}



void FaceDetector::setDev(const std::string& devid, const std::string& ipcid)
{
	dev_id_ = devid;
	ipc_id_ = ipcid;
}

void* FaceDetector::supwork(void* param)
{
	FaceDetector* self = static_cast<FaceDetector*>(param);
	if (self)
	{
		self->DecodeJpeg();
	}
}

void FaceDetector::DecodeJpeg()
{
	while (decode_flag_)
	{
		//OutputLog_Info("test");
		Frame* fm = NULL;
		pthread_mutex_lock(&mutex_frame_);
		if (!frame_set_.empty())
		{
			//OutputLog_Info("te");
			fm = frame_set_.front();
			frame_set_.erase(frame_set_.begin());
		}
		pthread_mutex_unlock(&mutex_frame_);
		if (fm == NULL)
		{
			//OutputLog_Info("no frame");
		}
		else
		{
			cv::Mat img;
			fm->DecodeFrame(img);
			RecvRtImg(img);
			fm->Clear();
			delete fm;
			fm = NULL;
		}
		usleep(10000);
	}
	
}

}

