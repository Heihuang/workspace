#include "SendVideo.h"
#include <unistd.h>
#include <stdio.h>
namespace rr
{


	SendVideo::SendVideo() :dev_id_(""), ipc_id_(""), thread_flag_(false), list_size_(25), fullview_sender_(NULL)
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		//dev_id_ = config.ReadString("DEV", "DevID", "005");
		//ipc_id_ = config.ReadString("DEV", "IpcID", "1");
		pthread_mutex_init(&mutex_img_, NULL);
		sem_init(&sem_img_, 0, 1);
		fullview_sender_ = new SendFullView();
	}

	SendVideo::~SendVideo()
	{
		pthread_mutex_destroy(&mutex_img_);
		sem_destroy(&sem_img_);
		printf("send video stop\n");
	}

	void* SendVideo::work(void* param)
	{
		SendVideo *self = static_cast<SendVideo*>(param);
		if (self)
		{
			self->SendJpeg();
		}
	}

	void SendVideo::SendJpeg()
	{
		if (fullview_sender_ == NULL)
		{
			return;
		}
		while (thread_flag_)
		{
			sem_wait(&sem_img_);
			std::vector<std::vector<unsigned char> > img_list;
			Copy(img_list);
			if (img_list.empty())
			{
				sleep(1);
				continue;
			}
			for (std::vector<std::vector<unsigned char> >::iterator it = img_list.begin(); it != img_list.end(); it++)
			{
				std::string cur_time = getCurTime();
				printf("cur_time=%s\n", cur_time.c_str());
				fullview_sender_->setData(ipc_id_, dev_id_, *it, cur_time);
				fullview_sender_->SendMsg();
				usleep(40000);
			}
			usleep(40000);
		}
	}
	void SendVideo::start()
	{
		thread_flag_ = true;
		pthread_create(&id_, NULL, work, this);
	}
	void SendVideo::stop()
	{
		thread_flag_ = false;
		PostSem();
	}
	void SendVideo::RevImg(const std::vector<unsigned char>& img)
	{
		pthread_mutex_lock(&mutex_img_);
		if (img_list_.size() > list_size_)
		{
			img_list_.clear();
		}
		else
		{
			img_list_.push_back(img);
		}
		pthread_mutex_unlock(&mutex_img_);
	}
	void SendVideo::Copy(std::vector<std::vector<unsigned char> > &img_list)
	{
		pthread_mutex_lock(&mutex_img_);
		img_list = img_list_;
		img_list_.clear();
		pthread_mutex_unlock(&mutex_img_);
	}
	void SendVideo::PostSem()
	{
		sem_post(&sem_img_);
	}
	void SendVideo::setIpcId(const std::string& ipid)
	{
		ipc_id_ = ipid;
	}	void SendVideo::setDevID(const std::string& devid)	{		dev_id_ = devid;	}
}


