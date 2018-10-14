#include "sendmessageMQ.h"
#include <string>
#include <vector>
#include <stdio.h>

namespace rr
{

BaseSendMessageMQ::BaseSendMessageMQ() : producer_(NULL), mqfactory_(NULL)
{
		
}

BaseSendMessageMQ::~BaseSendMessageMQ()
{
		
}


bool BaseSendMessageMQ::SendMsg()
{
	if (producer_)
	{
		MessageInfo msg;
		CreateMsg(msg);
		producer_->SendMsg(msg);
		return true;
	}
	else
	{
		return false;
	}
}

SendDeviceRegistMessageMQ::SendDeviceRegistMessageMQ() :devid_(""), devtype_(""), devmodel_(""), devip_("")
{
	mqfactory_ = new DeviceRegistProducerFactory();
	if (mqfactory_)
	{
		mqfactory_->CreateProducerProc(&producer_);
	}
}

SendDeviceRegistMessageMQ::~SendDeviceRegistMessageMQ()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendDeviceRegistMessageMQ::CreateMsg(MessageInfo &msg)
{
	msg.setDevId(devid_);
	msg.setDevType(devtype_);
	msg.setDevModel(devmodel_);
	msg.setDevIp(devip_);
	msg.setIPCId(ipcid_);
}

void SendDeviceRegistMessageMQ::RecvData(const std::string& devid, const std::string& devtype, const std::string& devmodel, const std::string& devip, const std::string& ipcid)
{
	devid_ = devid;
	devtype_ = devtype;
	devmodel_ = devmodel;
	devip_ = devip;
	ipcid_ = ipcid;
}
/*********************************************************************************************/

SendUsbCamaraRegistMessageMQ::SendUsbCamaraRegistMessageMQ() : ipc_id_(""), dev_id_("")
{
	//
	mqfactory_ = new UsbCamaraRegistProducerFactory();
	if (mqfactory_)
	{
		mqfactory_->CreateProducerProc(&producer_);
	}
}

SendUsbCamaraRegistMessageMQ::~SendUsbCamaraRegistMessageMQ()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendUsbCamaraRegistMessageMQ::CreateMsg(MessageInfo& msg)
{
	msg.setDevId(dev_id_);
	msg.setIPCId(ipc_id_);
}

void SendUsbCamaraRegistMessageMQ::RecvData(const std::string& ipcid, const std::string&devid)
{
	ipc_id_ = ipcid;
	dev_id_ = devid;
}
/*************************************************************************************************/

SendDeviceStatusUpdateMessageMQ::SendDeviceStatusUpdateMessageMQ() :dev_id_("")
{
	mqfactory_ = new DeviceStatusUpdateProducerFactory();
	if (mqfactory_)
	{
		mqfactory_->CreateProducerProc(&producer_);
	}
}

SendDeviceStatusUpdateMessageMQ::~SendDeviceStatusUpdateMessageMQ()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendDeviceStatusUpdateMessageMQ::CreateMsg(MessageInfo& msg)
{
	msg.setDevId(dev_id_);
}

void SendDeviceStatusUpdateMessageMQ::RecvData(const std::string&devid)
{
	dev_id_ = devid;
}


SendUsbStatusUpdateMessageMQ::SendUsbStatusUpdateMessageMQ() :ipc_id_("")
{
	mqfactory_ = new UsbCamaraStatusUpdateProducerFactory();
	if (mqfactory_)
	{
		mqfactory_->CreateProducerProc(&producer_);
	}
}

SendUsbStatusUpdateMessageMQ::~SendUsbStatusUpdateMessageMQ()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendUsbStatusUpdateMessageMQ::CreateMsg(MessageInfo& msg)
{
	msg.setIPCId(ipc_id_);
}

void SendUsbStatusUpdateMessageMQ::RecvData(const std::string&ipcid)
{
	ipc_id_ = ipcid;
}




}
