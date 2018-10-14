#include "FacesDetectorFdc.h"
#include <vector>
#include <iostream>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include "../baseutils/Mat2Img.h"
#include "../baseutils/RrConfig.h"
#include "../baseutils/GetCurTime.h"

namespace rr
{
FaceDetectorFdc::FaceDetectorFdc() :min_eye_dist_(0), min_face_width_(0), pitch_(0), yaw_(0), roll_(0), score_(0.0), fv_flag_(false), 
step_rect_(1), step_confidence_(1), step_definition_(1), step_yaw_(1), step_light_(1), step_pitch_(1), step_roll_(1), step_dinfoenable_(1), is_remove_face_(0), face_timeout_(0), weight_dif_(0), is_send_fullview_(0), fullview_h_(480), fullview_w_(640)
{
	RrConfig config;
	config.ReadConfig("config.ini");
	min_eye_dist_  = config.ReadInt("DETECT", "MinEyeDist", 30);
	min_face_width_ = config.ReadInt("DETECT", "MinFaceWidth", 50);
	pitch_ = config.ReadInt("DETECT", "MinPitch", 90);
	yaw_ = config.ReadInt("DETECT", "MinYaw", 90);
	roll_ = config.ReadInt("DETECT", "MinRoll", 90);
	score_ = config.ReadFloat("DETECT", "MinScore", 0.55);
	step_rect_ = config.ReadInt("PLAT", "StepRect", 1);
	step_confidence_ = config.ReadInt("PLAT", "StepConfidence", 1);
	step_yaw_ = config.ReadInt("PALT", "StepYaw", 1);
	step_roll_ = config.ReadInt("PALT", "StepRoll", 1);
	step_pitch_ = config.ReadInt("PALT", "StepPitch", 1);
	face_timeout_ = config.ReadInt("PALT", "FaceTimeOut", 1);
	is_remove_face_ = config.ReadInt("PALT", "IsRemoveFace", 1);
	weight_dif_ = config.ReadInt("PALT", "Dif", 30);
	is_send_fullview_ = config.ReadInt("FullView", "IsSendFullView", 0);
	fullview_w_ = config.ReadInt("FullView", "Width", 640);
	fullview_h_ = config.ReadInt("FullView", "Height", 480);
	std::string mode_dir = "models";
	detector_handle_ = rr_fd_create_detector(mode_dir.c_str(), 0);
	if (detector_handle_ == NULL)
	{
		std::cout << "create detector failed!!!" << std::endl;
		return;

	}
	std::cout << "MinEyeDist=" << min_eye_dist_ << std::endl;
	std::cout << "MinFaceWidth=" << min_face_width_ << std::endl;
	std::cout << "MinPitch=" << pitch_ << std::endl;
	std::cout << "MinYaw=" << yaw_ << std::endl;
	std::cout << "MinRoll=" << roll_ << std::endl;
	std::cout << "MinScore=" << score_ << std::endl;
}



FaceDetectorFdc::~FaceDetectorFdc()
{
	FaceDetector::wait();
	if (detector_handle_ != NULL)
	{
		rr_fd_destroy_detector(detector_handle_);
	}
	quality_into_.clear();
}

void FaceDetectorFdc::Run()
{
	while (thread_flag_)
	{
		int ret = 0;
		std::vector<cv::Mat> img_src;
		CopyImg(img_src);
		std::vector<cv::Mat>::iterator it = img_src.begin();
		while (it != img_src.end())
		{
			cv::Mat dst = (*it).clone();
			ret = FacesDetecte(dst);
			if (ret == -1)
			{
				break;
			}
			it++;
		}
		usleep(10);
	}
}



void FaceDetectorFdc::SendFace(const FaceInfo& faceinfo)
{
	if (rtface_sender_)
	{
		rtface_sender_->setData(faceinfo);
		rtface_sender_->SendMsg();
	}
}

void FaceDetectorFdc::SendFaceInfo(const std::string& faceid, const std::string& curtime, const std::string& ipcid, const std::string& devid)
{
	if (faceupdate_sender_)
	{
		faceupdate_sender_->setData(faceid, curtime, ipcid, devid);
		faceupdate_sender_->SendMsg();
	}
}

void FaceDetectorFdc::SendFullView(const std::vector<unsigned char>& blob, const std::string& cur_time)
{
	if (fullview_sender_)
	{
		fullview_sender_->setData(ipc_id_, dev_id_, blob, cur_time);
		fullview_sender_->SendMsg();
	}
}



int FaceDetectorFdc::FacesDetecte(const cv::Mat& img)
{
#if 1
	try
	{
		if (img.empty() || img.data == NULL || img.cols < 0 || img.rows < 0)
		{
			printf("img is error\n");
			return -1;
		}
		if ((img.rows / img.cols >= 30) || (img.cols / img.rows >= 30))
		{
			printf("img rows cols is error\n");
			return -1;
		}
		rr_face_t* faces_array = NULL;
		int faces_count = 0;
		int ret = 0;
		//cv::imshow("test", img);

		//cv::waitKey(3);
		std::string cur_time = getCurTime();
		printf("start_time=%s\n", cur_time.c_str());

		ret = rr_fd_detect(detector_handle_, img.data, RR_IMAGE_BGR8UC3, img.cols, img.rows, &faces_array, &faces_count);
		if (ret != RR_OK)
		{
			printf("detect failed!!!\n");
			rr_fd_release_detect_result(faces_array);
			return -1;
		}

		cur_time = getCurTime();
		printf("end_time=%s  face_count=%d\n", cur_time.c_str(), faces_count);
		//int index = 0;
		DoSomethings(img, faces_array, faces_count);
		rr_fd_release_detect_result(faces_array);
	}
	catch (...)
	{
		printf("detect exprection\n");
		return -1;
	}
	
#endif
	return 0;
}


int FaceDetectorFdc::getFace(const cv::Mat& face, rr_face_t rtface, cv::Mat &rface)
{

	if (rtface.rect.left <= 0 || rtface.rect.top <= 0) 
	{

		return 0;

	}

	if (rtface.rect.right >= face.cols || rtface.rect.bottom >= face.rows)
	{

		return 0;

	}

	int iFaceWight = rtface.rect.right - rtface.rect.left;

	int step = iFaceWight / 5;

	int left = rtface.rect.left - step > 0 ? rtface.rect.left - step : 1;

	int wight = rtface.rect.right + step < face.cols ? rtface.rect.right + step - left : face.cols - 1 - left;

	int top = rtface.rect.top - 2 * step > 0 ? rtface.rect.top - 2 * step : 1;

	int hight = rtface.rect.bottom + 2 * step < face.rows ? rtface.rect.bottom + 2 * step - top : face.rows - 1 - top;

	rface = face(cv::Rect(left, top, wight, hight));

	return 1;

}


void FaceDetectorFdc::DoSomethings(const cv::Mat& face, const rr_face_t *rtface, const int& count)
{

	for (int i = 0; i < count; i++)

	{


		// 矩形框的范围有超出图片范围的情况

		cv::Mat matFace;

		int fc = getFace(face, rtface[i], matFace);

		if (fc == 0)

		{

			continue;

		}

		QualityJudgment(matFace, face, rtface[i]);

	}

	for (std::map<int, QualityInfo>::iterator it = quality_into_.begin(); it != quality_into_.end(); it)
	{

		if (((getSecs() - it->second.start_time) > face_timeout_))
		{

			printf("clear a face faceid=%d\n", it->first);

			quality_into_.erase(it++);

		}
		else
		{

			it++;

		}

	}

}


bool FaceDetectorFdc::QualityJudgment(const cv::Mat& face, const cv::Mat& img, const rr_face_t& rtface)
{

	if (rtface.rect.left <= 0 || rtface.rect.top <= 0)
	{
		return false;
	}


#if 0


	if (face id 不存在)


	{


		保存 计算we并发送


	}


	else


	{


		if（权重大于已有Faceid的权重）


		{


			置换图片并发送


		}


		else


		{


			更新图片时间


		}


	}


	//faceid 首次时间距离当前超过3秒


#endif


	double dr = 0.0;

	double bl = 0.0;

	dr = CheckBrilliance(img); //

	bl = CheckDefinition(img); //清晰度

	FaceInfo face_info;

	std::string cur_time = getCurTime();

	printf("now_time=%d\n", getSecs());
	if (is_remove_face_)

	{

		int w = CalculatWeight(rtface, dr, bl);	

		QualityInfo qinfo;

		std::map<int, QualityInfo>::iterator it = quality_into_.find(rtface.id);

		if (it != quality_into_.end())

		{

			printf("w1=%d  w2=%d\n", w, it->second.weight);

			it->second.end_time = getSecs();

			if ((w - it->second.weight) > weight_dif_)

			{

				//CreateFaceInfo(img, face,face_info, rtface, dr, bl, cur_time, w);
				CreateFaceInfo(face,img,face_info, rtface, dr, bl, cur_time, w);
				SendFace(face_info);

				it->second.weight=w;

				printf("send better face faceid=%d\n", rtface.id);

			}

			else

			{

				//

				std::string faceid;

				rtFace2string(rtface.id, faceid);

				SendFaceInfo(faceid, cur_time, ipc_id_, dev_id_);

				printf("send update face faceid=%d\n", rtface.id);

			}

		}

		else
		{

			QualityInfo qinfo;

			qinfo.start_time = getSecs();

			qinfo.end_time = getSecs();

			qinfo.weight = w;

			//CreateFaceInfo(img, face, face_info, rtface, dr, bl, cur_time,w);
			CreateFaceInfo(face,img,face_info, rtface, dr, bl, cur_time, w);
			SendFace(face_info);

			quality_into_.insert(std::make_pair(rtface.id, qinfo));

			printf("send new face faceid=%d\n", rtface.id);

		}

	}

	else
	{

		CreateFaceInfo(img, face, face_info, rtface, dr, bl, cur_time,100);
		SendFace(face_info);

	}

	return true;

}


double FaceDetectorFdc::CheckBrilliance(const cv::Mat& input_img)

{

	cv::Mat hsv_img, v_img, mean, stdDev;

	cv::cvtColor(input_img, hsv_img, cv::COLOR_BGR2HSV);	// 颜色空间转换

	cv::extractChannel(hsv_img, v_img, 2);	// 提取第三通道(v通道)

	cv::meanStdDev(v_img, mean, stdDev);	// 计算均值和方差, 结果为1*1的矩阵

	return mean.at<double>(0, 0);	// 从1*1矩阵中提取唯一元素

}


double FaceDetectorFdc::CheckDefinition(const cv::Mat& input_img)
{

	return 0.0;

}





void FaceDetectorFdc::CreateFaceInfo(const cv::Mat& img, const cv::Mat& fullview, FaceInfo& faceinfo, const rr_face_t& rtface, const double& dr, const double& bl, const std::string& cur_time, const int& fweight)

{

	std::string facescore;

	std::string yaw; //

	std::string pitch;

	std::string roll;

	std::string faceid;

	std::string facewidth;

	std::string eyewidth;

	std::string definition;

	std::string brightness;

	std::string w;

	std::string facesize;

	rtFace2string(rtface.confidence, facescore);

	rtFace2string(rtface.yaw, yaw);

	rtFace2string(rtface.pitch, pitch);

	rtFace2string(rtface.roll, roll);

	rtFace2string(rtface.id, faceid);

	rtFace2string(rtface.rect.right - rtface.rect.left, facewidth);

	rtFace2string(rtface.rect.right - rtface.rect.left, eyewidth);

	rtFace2string(bl, definition);

	rtFace2string(dr, brightness);

	rtFace2string(fweight, w);

	rtFace2string(((rtface.rect.right - rtface.rect.left)*(rtface.rect.bottom - rtface.rect.top)), facesize);

	std::vector <unsigned char> face;
	std::vector <unsigned char> fv;
	Mat2Img img_tool;

	img_tool.MatToByteArray(img, face);
	
	if (is_send_fullview_ == 1)
	{
		cv::Mat dst;
		cv::resize(fullview, dst, cv::Size(fullview_w_, fullview_h_), 0, 0);
		img_tool.MatToByteArray(dst, fv);
	}
	faceinfo.face = face;
	faceinfo.fullview = fv;
	faceinfo.facescore = facescore;

	faceinfo.yaw = yaw;

	faceinfo.pitch = pitch;

	faceinfo.roll = roll;

	faceinfo.faceid = faceid;

	faceinfo.facewidth = facewidth;

	faceinfo.eyewidth = eyewidth;

	faceinfo.definition = definition;

	faceinfo.curtime = cur_time;

	faceinfo.IPCId = ipc_id_;

	faceinfo.devId = dev_id_;

	faceinfo.brightness = brightness;

	faceinfo.facequalityscore = w;

	faceinfo.facesize = facesize;

	faceinfo.talkid = talk_id_;

}





void FaceDetectorFdc::rtFace2string(const int& rt, std::string& tmp)

{


	char temp_buf[256] = { 0 };

	sprintf(temp_buf, "%d", rt);

	tmp.assign(temp_buf);

	memset(temp_buf, 0, 256);

}


void FaceDetectorFdc::rtFace2string(const float& rt, std::string& tmp)

{


	char temp_buf[256] = { 0 };

	sprintf(temp_buf, "%f", rt);

	tmp.assign(temp_buf);

	memset(temp_buf, 0, 256);

}





void FaceDetectorFdc::rtFace2string(const double& rt, std::string& tmp)
{


	char temp_buf[256] = { 0 };


	sprintf(temp_buf, "%f", rt);


	tmp.assign(temp_buf);


	memset(temp_buf, 0, 256);


}





int FaceDetectorFdc::CalculatWeight(const rr_face_t& rtface, const double& dr, const double& bl)

{

	int w = 0;

	int rect_w = 0;

	int roll_w = 0;

	int pitch_w = 0;

	int yaw_w = 0;

	int light_w = 0;

	int confidence_w = 0;


	/*


	if (CalMinFace(rtface) == false)


	{


		return w;


	}


	*/


	//yaw
	rect_w = CalRectWeight(abs((rtface.rect.right - rtface.rect.left)*(rtface.rect.bottom - rtface.rect.top)));
	roll_w = CalRollWeight(abs(rtface.roll));
	pitch_w = CalPitchWeight(abs(rtface.pitch));
	yaw_w = CalYawWeight(abs(rtface.yaw));
	light_w = CalLightWeight((int)dr);
	confidence_w = CalConfidenceWeight((int)rtface.confidence);
	w = rect_w + roll_w + pitch_w + yaw_w + light_w + confidence_w;
	return w;

}



int FaceDetectorFdc::CalRectWeight(const int& rect)
{
	int w = 0;
	switch (rect / step_rect_)
	{

	case 0:

		w = 0;

		break;

	case 1:

		w = 20;

		break;

	case 2:

		w = 30;

		break;

	case 3:

		w = 50;

		break;

	default:

		w = 100;

		break;

	}

	return w;

}


int FaceDetectorFdc::CalRollWeight(const int& roll)
{

	int w = 0;
	switch (roll / step_roll_)
	{
	case 0:
		w = 100;
		break;
	case 1:

		w = 80;

		break;

	case 2:

		w = 50;

		break;

	case 3:

		w = 20;

		break;

	default:

		w = 10;

		break;

	}

	return w;

}


int FaceDetectorFdc::CalYawWeight(const int& yaw)

{

	int w = 0;

	switch (yaw / step_yaw_)

	{

	case 0:

		w = 100;

		break;

	case 1:

		w = 80;

		break;

	case 2:

		w = 50;

		break;

	case 3:

		w = 20;

		break;

	default:

		w = 10;

		break;

	}

	return w;

}





int FaceDetectorFdc::CalPitchWeight(const int& pitch)

{

	int w = 0;

	switch (pitch / step_pitch_)

	{

	case 0:

		w = 100;

		break;

	case 1:

		w = 80;

		break;

	case 2:

		w = 50;

		break;

	case 3:

		w = 20;

		break;

	default:

		w = 10;

		break;

	}

	return w;

}





int FaceDetectorFdc::CalConfidenceWeight(const int& confidence)

{

	int w = 0;

	switch (confidence / step_confidence_)

	{

	case 0:

		w = 0;

		break;

	case 1:

		w = 10;

		break;

	case 2:

		w = 20;

		break;

	case 3:

		w = 30;

		break;

	default:

		w = 50;

		break;

	}

	return w;

}





int FaceDetectorFdc::CalLightWeight(const int& light)

{


	int w = 0;

	switch (light / step_light_)

	{


	case 0:


		w = 0;


		break;


	case 1:


		w = 10;


		break;


	case 2:


		w = 20;


		break;


	case 3:


		w = 30;


		break;


	default:


		w = 50;


		break;


	}


	return w;


}





bool FaceDetectorFdc::CalMinFace(const rr_face_t& rtface)
{


	if (rtface.rect.right - rtface.rect.left < min_face_width_)
	{


		return false;


	}

	if (rtface.confidence < score_)
	{
		return false;
	}

	if (!(rtface.pitch < pitch_ && rtface.pitch >(-1) * pitch_))
	{
		return false;
	}

	if (!(rtface.yaw < yaw_ && rtface.yaw >(-1) * yaw_))
	{


		return false;


	}


	if (!(rtface.roll < roll_ && rtface.roll > roll_))


	{


		return false;


	}


	return true;


}





}





