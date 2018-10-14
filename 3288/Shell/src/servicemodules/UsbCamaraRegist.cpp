#include "UsbCamaraRegist.h"

namespace rr
{

UsbCamaraRegist::UsbCamaraRegist() :dev_id_(""), ipc_id_(""), sender_(NULL)
{
	sender_ = new SendUsbCamaraRegistMessageMQ();
}

UsbCamaraRegist::~UsbCamaraRegist()
{
	if (sender_)
	{
		delete sender_;
		sender_ = NULL;
	}
}

void UsbCamaraRegist::Regist()
{
	if (sender_)
	{
		sender_->RecvData(ipc_id_, dev_id_);
		sender_->SendMsg();
	}
}

void UsbCamaraRegist::setDevId(const std::string& devid)
{
	dev_id_ = devid;
}
}
