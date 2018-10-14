#include "DeviceStatusUpdate.h"
namespace rr
{
DeviceStatusUpdate::DeviceStatusUpdate() : dev_id_(""), sender_(NULL)
{
	sender_ = new SendDeviceStatusUpdateMessageMQ();
}

DeviceStatusUpdate::~DeviceStatusUpdate()
{
}

void DeviceStatusUpdate::Update()
{
	SendAliveMsg();
}

void DeviceStatusUpdate::SendAliveMsg()
{
	if (sender_)
	{
		sender_->RecvData(dev_id_);
		sender_->SendMsg();
	}
}

void DeviceStatusUpdate::setDevId(const std::string& devid)
{
	dev_id_ = devid;
}

}

