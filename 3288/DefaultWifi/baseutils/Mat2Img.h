#ifndef MAT2IMG_H
#define MAT2IMG_H
#include <opencv.hpp>
#include <core/core.hpp>
#include <imgproc/imgproc.hpp>
#include <highgui/highgui.hpp>
#include <vector>
namespace rr
{
	class Mat2Img 
	{
		
	public:
		 Mat2Img();
		 ~Mat2Img();
	public:
		int MatToByteArray(const cv::Mat mat, std::vector<unsigned char>& buff)
		{
			if (mat.empty()) {
				return 0;
			}
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
			if ((buff[0] == 0xFF && buff[1] == 0xD8))
			{
				matImage = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
			}
			else
			{
				buff.insert(buff.begin(), 0xFF);
				buff.insert(buff.begin()+1, 0xD8);
				matImage = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
			}
			return 0;
		}
	};
}
#endif