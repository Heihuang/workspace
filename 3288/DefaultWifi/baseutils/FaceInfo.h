#ifndef FACEINFO_H_
#define FACEINFO_H_
#include <string>
#include <vector>
namespace rr
{

typedef struct _FaceInfo
{
	_FaceInfo(){}
	~_FaceInfo(){}
	std::string taskid;
	std::vector<unsigned char> face;
	std::string facescore;
	std::string yaw;
	std::string pitch;
	std::string roll;
	std::string faceid;
	std::string facewidth;
	std::string eyewidth;
	std::string definition;
	std::string brightness;
	std::string curtime;
	std::string IPCId;
	std::string devId;
	std::string facequalityscore;
	std::string facesize;
	std::string talkid;
}FaceInfo;

}
#endif
