#include "Recvmessagemq.h"
#include <string>
#include <vector>

namespace rr
{
BaseRecvmessageMQ::BaseRecvmessageMQ() :consumer_(NULL), mqfactory_(NULL), thread_flag_(false), dev_id_("")
{
		
}

BaseRecvmessageMQ::~BaseRecvmessageMQ()
{

}

void BaseRecvmessageMQ::CreateConsumer()
{
	mqfactory_->CreateConsumerProc(&consumer_);
}

void BaseRecvmessageMQ::DestroyConsumer()
{
	if (consumer_ && mqfactory_)
	{
		mqfactory_->DestroyConsumerProc(consumer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void BaseRecvmessageMQ::setDevId(const std::string& devid)
{
	dev_id_ = devid;
}


FullViewNotifyMQ::FullViewNotifyMQ() :manage_cm_(NULL)
{
	mqfactory_ = new FullViewNotifyConsumerFactory();
	if (mqfactory_ == NULL)
	{
		return;
	}
	CreateConsumer();
}

FullViewNotifyMQ::~FullViewNotifyMQ()
{
	DestroyConsumer();
}

void FullViewNotifyMQ::Run()
{
	if (consumer_)
	{
		consumer_->run();
	}
}

void FullViewNotifyMQ::RegisterMQ()
{
	if (consumer_)
	{
		consumer_->RegUser(this);
		consumer_->RegCallBack(CBMessage);
	}
}

void FullViewNotifyMQ::RecvMsg(const std::string& devid, const std::string& ipcid, const std::string& IsSend)
{
	if (devid != dev_id_)
	{
		printf("c1=%s, c2=%s \n", devid.c_str(), dev_id_.c_str());
		return;
	}
	if (manage_cm_)
	{
		Camera* c = manage_cm_->getCamera(ipcid);
		if (c == NULL)
		{
			printf("find not camera\n");
			return;
		}
		UsbCamera *usbc = dynamic_cast<UsbCamera*>(c);
		IPCamera *ipc = dynamic_cast<IPCamera*>(c);
		if (usbc)
		{
			usbc->setVideoFlag(IsSend, ipcid, dev_id_);
			printf("video is %s is=%s", ipcid.c_str(), IsSend.c_str());
		}
		if (ipc)
		{
		}
		
	}
}

void __stdcall FullViewNotifyMQ::CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user)
{
	FullViewNotifyMQ* self = static_cast<FullViewNotifyMQ*>(user);
	if (self)
	{
		std::string devid;
		std::string ipcid;
		std::string issend;
		std::map<std::string, std::vector<unsigned char> >::iterator it = msg.begin();
		while (it != msg.end())
		{
			if (it->first == "DevID")
			{
				devid.assign(it->second.begin(), it->second.end());
			}
			else if (it->first == "IPC_ID")
			{
				ipcid.assign(it->second.begin(), it->second.end());
			}
			else if (it->first == "IsSend")
			{
				issend.assign(it->second.begin(), it->second.end());
			}
			printf("key=%s\n", it->first.c_str());
			it++;
		}
		self->RecvMsg(devid, ipcid, issend);
	}
}

void* FullViewNotifyMQ::RecvThread(void* param)
{
	FullViewNotifyMQ* self = static_cast<FullViewNotifyMQ*>(param);
	if (self)
	{
		self->Run();
	}
}

void FullViewNotifyMQ::Start()
{
	pthread_create(&id_, NULL, RecvThread, this);
	printf("FullViewNotify thread\n");
}

void FullViewNotifyMQ::Stop()
{
	pthread_join(id_, NULL);
}

void FullViewNotifyMQ::setManageCamera(ManageCamera* manage_cm)
{
	manage_cm_ = manage_cm;
}

/*************************************************************************************************/
IpcCameraNotifyMQ::IpcCameraNotifyMQ() :manage_cm_(NULL)
{
	mqfactory_ = new UpdateIpcConsumerFactory();
	if (mqfactory_ == NULL)
	{
		return;
	}
	CreateConsumer();
}

IpcCameraNotifyMQ::~IpcCameraNotifyMQ()
{
	DestroyConsumer();
}

void IpcCameraNotifyMQ::Run()
{
	if (consumer_)
	{
		consumer_->run();
	}
}

void IpcCameraNotifyMQ::RegisterMQ()
{
	if (consumer_)
	{
		consumer_->RegUser(this);
		consumer_->RegCallBack(CBMessage);
	}
}

void IpcCameraNotifyMQ::RecvMsg(const std::string& devid, const std::string& ipcid, const std::string& ipc_url, const std::string& ipc_ip, const int& ipc_port, const std::string&ipc_No, const int& isdel)
{
	if (devid != dev_id_)
	{
		printf("c1=%s, c2=%s \n", devid.c_str(), dev_id_.c_str());
		return;
	}
	if (isdel == 1)
	{
		manage_cm_->DelIpc(ipcid);
	}
	else if (isdel == 0)
	{
		manage_cm_->UpdataIpc(ipcid, ipc_url, ipc_ip, ipc_port, ipc_No);
	}
}

void __stdcall IpcCameraNotifyMQ::CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user)
{
	IpcCameraNotifyMQ* self = static_cast<IpcCameraNotifyMQ*>(user);
	if (self == NULL)
	{
		return;
	}
	std::string ipcid;
	std::string ipc_url;
	std::string ipc_ip = "0.0.0.0";
	std::string devId;
	std::string ipcno;
	int ipc_port = 0;
	int is_delete = 0;
	std::map<std::string, std::vector<unsigned char> >::const_iterator it = msg.begin();
	while (it != msg.end())
	{
		std::string temp;
		if (it->first == "IPC_ID")
		{
			ipcid.assign(it->second.begin(), it->second.end());
		}
		else if (it->first == "IpcUrl")
		{
			ipc_url.assign(it->second.begin(), it->second.end());
		}
		else if (it->first == "IpcIp")
		{
			ipc_ip.assign(it->second.begin(), it->second.end());
		}
		else if (it->first == "DevID")
		{
			devId.assign(it->second.begin(), it->second.end());
		}
		else if (it->first == "IpcPort")
		{
			temp.assign(it->second.begin(), it->second.end());
			ipc_port = atoi(temp.c_str());
		}
		else if (it->first == "isDelete")
		{
			temp.assign(it->second.begin(), it->second.end());
			is_delete = atoi(temp.c_str());
		}
		printf("key=%s\n", it->first.c_str());
		it++;
	}
	self->RecvMsg(devId, ipcid, ipc_url, ipc_ip, ipc_port, ipcno, is_delete);
}

void* IpcCameraNotifyMQ::RecvThread(void* param)
{
	IpcCameraNotifyMQ* self = static_cast<IpcCameraNotifyMQ*>(param);
	if (self)
	{
		self->Run();
	}
}

void IpcCameraNotifyMQ::Start()
{
	pthread_create(&id_, NULL, RecvThread, this);
	printf("start detect thread\n");
}

void IpcCameraNotifyMQ::Stop()
{
	pthread_join(id_, NULL);
}

void IpcCameraNotifyMQ::setManageCamera(ManageCamera* manage_cm)
{
	manage_cm_ = manage_cm;
}

}
