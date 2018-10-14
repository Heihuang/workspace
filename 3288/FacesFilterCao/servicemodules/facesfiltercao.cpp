#include "facesfiltercao.h"
#include "../baseutils/GetCurTime.h"
#include <stdio.h>
#include <string>
FacesFilterCao::FacesFilterCao()
{
    std::string mode_dir = "models";
    detector_handle_ = rr_fd_create_detector(mode_dir.c_str(), 0);
    if (detector_handle_ == NULL)
    {
        printf("create detector failed!!!\n");
        return;
    }
	 verifier_handle_ = rr_fv_create_verifier(mode_dir.c_str(), 0);
    if (verifier_handle_ == NULL)
    {
        printf("rr_fv_create_verifier() failed!\n");
	return;
    }
}

FacesFilterCao::~FacesFilterCao()
{
     if (detector_handle_ != NULL)
     {
        rr_fd_destroy_detector(detector_handle_);
     }
     if(verifier_handle_ != NULL)
     {
	rr_fv_destroy_verifier(verifier_handle_); 
     }
}

int FacesFilterCao::ExtractFeature(const cv::Mat &image, rr_feature_t *feature)
{
    if(image.empty())
    {
        return 0;
    }
    rr_face_t* faces_array = NULL;
    int faces_count = 0;
    int ret = 0;
   
#if 1
    std::string cur_time = rr::getCurTime();
    printf("start_time=%s\n", cur_time.c_str());
#endif
    ret = rr_fd_detect(detector_handle_, image.data, RR_IMAGE_BGR8UC3, image.cols, image.rows, &faces_array, &faces_count);
    if (ret != RR_OK)
    {
        printf("detect failed!!!\n");
        return -1;
    }
#if 1
    if (faces_count == 0)
    {
        rr_fd_release_detect_result(faces_array);
        cur_time = rr::getCurTime();
        printf("end_time=%s  face_count=%d\n", cur_time.c_str(), faces_count);
        return -1;
    }
#endif
#if 1
        cur_time = rr::getCurTime();

#endif
    ret = rr_fv_extract_feature(verifier_handle_, image.data, RR_IMAGE_BGR8UC3, image.cols, image.rows, faces_array, feature);
    rr_fd_release_detect_result(faces_array);
	
  if (ret != RR_OK)
    {
	printf("rr_fv_extract_feature() failed: %d\n", ret);   
        return -1;	
    }
    printf("end_time=%s  face_count=%d\n", cur_time.c_str(), faces_count);
    return 0;
}

void FacesFilterCao::FilterFace(const cv::Mat &facedata, const std::string &taskid)
{
    rr_feature_t cur_feature;
    std::string str = rr::getCurTime();
    printf("start filter = %s\n", str.c_str());
    std::vector<unsigned char> fea;
    std::string fea_start = "false";
    int ret = ExtractFeature(facedata, &cur_feature);
	
    if(ret != 0)
    {
        if(sender_)
        {
            fea_start = "false";
            sender_->RecvData(taskid, fea,fea_start);
            sender_->SendMsg();
        }
        return ;
    }
	                                      
    {
        if(sender_)
        {
            fea_start = "true";
	    fea.assign((unsigned char *)&cur_feature, (unsigned char *)(&cur_feature)+sizeof(rr_feature_t));
            sender_->RecvData(taskid, fea,fea_start);
            sender_->SendMsg();
        }
    }
    fea.clear();
    str = rr::getCurTime();
    printf("end filter = %s\n", str.c_str());
}

