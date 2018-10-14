#ifndef DEVICE_STATUS_UPDATE_H_
#define DEVICE_STATUS_UPDATE_H_
#include <string>
#include "../message/sendmessageMQ.h"

namespace rr
{

class DeviceStatusUpdate
{
public:
	DeviceStatusUpdate();
	~DeviceStatusUpdate();
	void Update();
	void SendAliveMsg();
	void setDevId(const std::string& devid);
private:
	std::string dev_id_;
	SendDeviceStatusUpdateMessageMQ *sender_;
};

}
#endif
