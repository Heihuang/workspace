#ifndef SEND_VIDEO_H_
#define SEND_VIDEO_H_
#include <vector>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include "../baseutils/RrConfig.h"
#include "../baseutils/GetCurTime.h"
#include "../message/SendmessageMQ.h"

namespace rr
{
	class SendVideo
	{
	public:
		SendVideo();
		~SendVideo();
		static void* work(void* param);
		void SendJpeg();
		void start();
		void stop();
		void RevImg(const std::vector<unsigned char>& img);
		void PostSem();
		void setIpcId(const std::string& ipid);		void setDevID(const std::string& devid);
	private:
		void Copy(std::vector<std::vector<unsigned char> > &img_list);
		std::string ipc_id_;
		std::string dev_id_;
		bool thread_flag_;
		int list_size_;
		std::vector<std::vector<unsigned char> > img_list_;
		pthread_mutex_t mutex_img_;
		pthread_t id_;
		sem_t sem_img_;
		SendFullView *fullview_sender_;
	};
}
#endif
