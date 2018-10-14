#ifndef DEVICE_REGIST_H_
#define DEVICE_REGIST_H_
#include <string>
#include "../message/sendmessageMQ.h"

namespace rr
{
class DeviceRegist
{
public:
	DeviceRegist();
	~DeviceRegist();
	void Regist();
	void setDevId(const std::string& devid);
	void setIP(const std::string& ip);
	void setIPCId(const std::string& ipcid);
private:
	std::string devid_;
	std::string localip_;
	std::string devtype_;
	std::string devmodel_;
	std::string ipc_id_;
	SendDeviceRegistMessageMQ* sender_;
};
}
#endif
