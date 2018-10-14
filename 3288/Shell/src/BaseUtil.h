#ifndef BASE_UTILS_H_
#define BASE_UTILS_H_
#include <vector>
#include <string>
namespace rr
{

struct VideoCaptureDevice
{
	std::wstring friendlyname;
	std::wstring description;
	std::wstring pidvid;
};
void GetFirstMacAddress(std::vector<std::string> &macs);
void GetLocalIP(std::string& ip);
}
#endif
