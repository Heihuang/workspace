#ifndef CAMERA_H_
#define CAMERA_H_
#include "FacesDetector.h"
#include "FactoryAlgorithm.h"
#include "FactoryAlgorithmfdc.h"
#include <string>
namespace rr
{

class Camera
{
public:
	Camera();
	virtual ~Camera();
	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual void Run() = 0;
	void setDev(const std::string& devid, const std::string& ipcid);
	void setVideoFlag(const std::string& flag, const std::string& ipcid, const std::string& devid);
	virtual bool getQuitState()const;
protected:
	FaceDetector* face_detector_;
	FactoryAlgorithm *factory_;
	int rate_;
	int video_w_;
	int video_h_;
	bool video_flag_;
	std::string ipc_id_;
	std::string dev_id_;
};
}
#endif
