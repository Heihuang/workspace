#ifndef FACE_DETECTOR_H_
#define FACE_DETECTOR_H_
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <sched.h>
#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
#include "../message/SendmessageMQ.h"
#include "../baseutils/Mat2Img.h"
#include "../../logprocessor.h"

namespace rr
{
	typedef struct _Frame 
	{
	public:
		_Frame() :buff_(NULL), size_(0)
		{

		}
		~_Frame(){}
		void CreateFrame(unsigned char* buff, const long& size)
		{
			if (buff_ == NULL)
			{
				buff_ = new unsigned char[size];
				memset(buff_, NULL, size);
				memcpy(buff_, buff, size);
				size_ = size;
			}
			else
			{
				delete[] buff_;
				buff_ = NULL;
				buff_ = new unsigned char[size];
				memset(buff_, NULL, size);
				memcpy(buff_, buff, size);
			}
		}
		void DecodeFrame(cv::Mat & img)
		{
			if (buff_ == NULL)
			{
				return;
			}
			std::vector<unsigned char> data;
			data.assign(buff_, buff_ + size_);
			//OutputLog_Info("cap buf size=%d", size_);
			//OutputLog_Info("jpeg in");
			img_decoder_.JPEGToMat(img, data);
			//OutputLog_Info("jpeg out");
		}
		void Clear()
		{
			if (buff_)
			{
				delete[] buff_;
				buff_ = NULL;
				size_ = 0;
			}
		}
	private:
		unsigned char* buff_;
		long size_;
		Mat2Img img_decoder_;
	}Frame;

	class FaceDetector
	{
	protected:
		std::vector<cv::Mat> img_list_;
		bool thread_flag_;
	public:
		FaceDetector();
		virtual ~FaceDetector();
		void RecvRtImg(const cv::Mat& img);
		void CopyImg(std::vector<cv::Mat>& img_list);
		virtual void Run() = 0;
		void start();
		void stop();
		void wait();
		void setDev(const std::string& devid, const std::string& ipcid);
		void RecvRtImg(unsigned char* buff, const long& size);
		void DecodeJpeg();
	private:
		static void* work(void* param);
		static void* supwork(void* param);
	private:
		pthread_mutex_t mutex_img_;
		pthread_mutex_t mutex_frame_;
		pthread_t id_detect_;
		pthread_t id_decode_;
		int list_size_;
		sem_t over_;
		bool decode_flag_;
		std::vector<Frame*> frame_set_;
	protected:
		SendFullView *fullview_sender_;
		SendRtFace *rtface_sender_;
		SendFaceUpdate* faceupdate_sender_;
		std::string ipc_id_;
		std::string dev_id_;
		std::string talk_id_;
	};
}

#endif





