#ifndef FACESFILTERCAO_H
#define FACESFILTERCAO_H
#include "basefacesfilter.h"
#include "fdc/rr_facedetect.h"
#include "fdc/rr_common.h"
#include "fdc/rr_faceverify.h"
#include "../message/sendmessageMQ.h"

class FacesFilterCao : public BaseFacesFilter
{
public:
    FacesFilterCao();
    ~FacesFilterCao();
    virtual void FilterFace(const cv::Mat& facedata, const std::string & taskid);
private:
    rr_handle_t detector_handle_;
    rr_handle_t verifier_handle_;
    int ExtractFeature(const cv::Mat &image, rr_feature_t *feature);
};

#endif // FACESFILTERCAO_H
