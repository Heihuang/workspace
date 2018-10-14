#include "DeviceRegist.h"
#include "../BaseUtil.h"
#include "../RrConfig.h"
namespace rr
{
DeviceRegist::DeviceRegist() :sender_(NULL), devid_(""), localip_(""), devtype_(""), devmodel_(""), ipc_id_("0")
{
	RrConfig config;
	config.ReadConfig("config.ini");
	devtype_ = config.ReadString("DEV", "DevType", "01");
	devmodel_ = config.ReadString("DEV", "DevModel", "0101");
	sender_ = new SendDeviceRegistMessageMQ();
}

DeviceRegist::~DeviceRegist()
{
	if (sender_)
	{
		delete sender_;
		sender_ = NULL;
	}
}

void DeviceRegist::Regist()
{
	if (sender_)
	{
		sender_->RecvData(devid_, devtype_, devmodel_, localip_, ipc_id_);
		sender_->SendMsg();
	}
}

void DeviceRegist::setDevId(const std::string& devid)
{
	devid_ = devid;
}

void DeviceRegist::setIP(const std::string& ip)
{
	localip_ = ip;
}

void DeviceRegist::setIPCId(const std::string& ipcid)
{
	ipc_id_ = ipcid;
}

}

