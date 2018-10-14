#include "UsbUpdate.h"

namespace rr
{

	UsbUpdate::UsbUpdate() :ipc_id_(""), sender_(NULL), flag_(false)
	{
		sender_ = new SendUsbStatusUpdateMessageMQ();
		if (sender_ == NULL)
		{
			return;
		}
	}

	UsbUpdate::~UsbUpdate()
	{
		if (sender_)
		{
			delete sender_;
			sender_ = NULL;
		}
	}

	void UsbUpdate::Update()
	{
		while (flag_)
		{
			SendAliveMsg();
			sleep(60);
		}
		
	}

	void UsbUpdate::SendAliveMsg()
	{
		if (sender_)
		{
			sender_->RecvData(ipc_id_);
			sender_->SendMsg();
			printf("Alive\n");
		}
	}

	void UsbUpdate::setIPCId(const std::string& ipcid)
	{
		ipc_id_ = ipcid;
	}

	void UsbUpdate::Stop()
	{
		flag_ = false;
		pthread_join(thread_id_, NULL);
	}

	void UsbUpdate::Start()
	{
		unsigned int id = 0;
		flag_ = true;
		pthread_create(&thread_id_, NULL, SendThread, this);
	}

	void* UsbUpdate::SendThread(void* param)
	{
		UsbUpdate* self = static_cast<UsbUpdate*>(param);
		if (self)
		{
			self->Update();
		}
	}

}

