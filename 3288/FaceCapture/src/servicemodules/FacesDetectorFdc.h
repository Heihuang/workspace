#ifndef FACE_VERIFYCAO_H_

#define FACE_VERIFYCAO_H_

#include "FacesDetector.h"
#include <vector>
#include <string>
#include <map>
#include "fdc/rr_facedetect.h"
#include "fdc/rr_common.h"
#include "../baseutils/FaceInfo.h"

namespace rr
{

typedef struct _QualityInfo
{

	int weight;

	int count;

	int start_time;

	int end_time;

}QualityInfo;



class FaceDetectorFdc : public FaceDetector
{

public:

	FaceDetectorFdc();

	virtual ~FaceDetectorFdc();

	virtual void Run();

private:

	int FacesDetecte(const cv::Mat& img);

	int getFace(const cv::Mat& face, rr_face_t rtface, cv::Mat &rface);

	void DoSomethings(const cv::Mat& face, const rr_face_t *rtface, const int& count);

	bool QualityJudgment(const cv::Mat& face, const cv::Mat& img, const rr_face_t& rtface);

	double CheckBrilliance(const cv::Mat& input_img);

	double CheckDefinition(const cv::Mat& input_img);

	void CreateFaceInfo(const cv::Mat& img, const cv::Mat& fullview, FaceInfo& faceinfo, const rr_face_t& rtface, const double& dr, const double& bl, const std::string& cur_time, const int& fweight);

	void rtFace2string(const int& rt, std::string& tmp);

	void rtFace2string(const float& rt, std::string& tmp);

	void rtFace2string(const double& rt, std::string& tmp);

	void SendFace(const FaceInfo& faceinfo);

	void SendFaceInfo(const std::string& faceid, const std::string& curtime, const std::string& ipcid, const std::string& devid);

	void SendFullView(const std::vector<unsigned char>& blob, const std::string& cur_time);

	int CalculatWeight(const rr_face_t& rtface, const double& dr, const double& bl);

	int CalRectWeight(const int& rect);

	int CalRollWeight(const int& roll);

	int CalYawWeight(const int& yaw);

	int CalPitchWeight(const int& pitch);

	int CalConfidenceWeight(const int& confidence);

	int CalLightWeight(const int& light);

	bool CalMinFace(const rr_face_t& rtface);

private:

	rr_handle_t detector_handle_;
	int min_eye_dist_;
	int min_face_width_;
	int pitch_;
	int yaw_;
	int roll_;
	float score_;
	bool fv_flag_;
	std::map<int, QualityInfo> quality_into_;
	int step_rect_;
	int step_confidence_;
	int step_dinfoenable_;
	int step_yaw_;
	int step_light_;
	int step_pitch_;
	int step_roll_;
	int step_definition_;
	int is_remove_face_;
	int face_timeout_;
	int weight_dif_;
	int is_send_fullview_;
	int fullview_w_;
	int fullview_h_;
};

}

#endif



