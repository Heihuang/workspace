#ifndef BASEFACESFILTER_H
#define BASEFACESFILTER_H
#include <string>
#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
#include "highgui.h"
#include "../message/sendmessageMQ.h"

class BaseFacesFilter
{
public:
    BaseFacesFilter();
    virtual ~BaseFacesFilter();
    virtual void FilterFace(const cv::Mat& facedata, const std::string & taskid) = 0;
protected:
    SendMessageMQ *sender_;
};
#endif // BASEFACESFILTER_H
