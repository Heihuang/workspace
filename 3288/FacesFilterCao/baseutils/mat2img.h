#ifndef MAT2IMG_H
#define MAT2IMG_H

#include "cv.h"
#include "opencv.hpp"
#include "cxcore.hpp"
#include <vector>

class Mat2Img
{
public:
    explicit Mat2Img();
    ~Mat2Img(){}
public:
    int MatToQByteArray(const cv::Mat mat, std::vector<uchar> &qbytearray)
    {
        if (mat.empty()) {
            return 0;
        }
        std::vector<uchar> buff;
        std::vector<int> param = std::vector<int>(2);
        param[0] = CV_IMWRITE_JPEG_QUALITY;
        param[1] = 95; // default(95) 0-100
        cv::imencode(".jpg", mat, buff, param);
        return 0;
    }

    int JPEGToMat(cv::Mat &matImage, std::vector<unsigned char> buff)
    {
        if (buff.empty()) {
            return 0;
        }
        matImage = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
        return 0;
    }
};

#endif // MAT2IMG_H
