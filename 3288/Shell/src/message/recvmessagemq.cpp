#include "recvmessagemq.h"
#include <string>
#include <vector>

namespace rr
{

	BaseRecvmessageMQ::BaseRecvmessageMQ() :consumer_(NULL), mqfactory_(NULL), thread_flag_(false), thread_id_(NULL), dev_id_(""), dev_update_(NULL)
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
			consumer_->Close();
			mqfactory_->DestroyConsumerProc(consumer_);
			delete mqfactory_;
			mqfactory_ = NULL;
		}
	}

	void BaseRecvmessageMQ::setDevUpdate(const DeviceStatusUpdate* devupdate)
	{
		dev_update_ = const_cast<DeviceStatusUpdate*>(devupdate);
	}

	/*************************************************************************************************/
	RecvRtmessageMQ::RecvRtmessageMQ()
	{
		mqfactory_ = new DeviceStatusUpdateRequestConsumerFactory();
		if (mqfactory_ == NULL)
		{
			return;
		}
		CreateConsumer();
	}

	RecvRtmessageMQ::~RecvRtmessageMQ()
	{
		DestroyConsumer();
	}

	void RecvRtmessageMQ::RegisterMQ()
	{
		if (consumer_)
		{
			consumer_->RegUser(this);
			consumer_->RegCallBack(CBMessage);
		}
	}

	void RecvRtmessageMQ::RecvMsg(const std::string& devid)
	{
		if (dev_id_ != devid)
		{
			printf("d1=%s, d2=%s\n", dev_id_.c_str(), devid.c_str());
			return;
		}
		if (dev_update_)
		{
			dev_update_->Update();
		}
	}
	void __stdcall RecvRtmessageMQ::CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user)
	{
		RecvRtmessageMQ* self = static_cast<RecvRtmessageMQ*>(user);
		if (self)
		{
			std::map<std::string, std::vector<unsigned char> >::iterator it = msg.begin();
			std::string devid;
			while (it != msg.end())
			{
				if (it->first == "DevID")
				{
					devid.assign(it->second.begin(), it->second.end());
				}
				it++;
			}
			self->RecvMsg(devid);
		}
	}

	void* RecvRtmessageMQ::RecvThread(void* param)
	{
		RecvRtmessageMQ* self = static_cast<RecvRtmessageMQ*>(param);
		if (self)
		{
			self->Run();
		}
		return 0;
	}

	void RecvRtmessageMQ::Start()
	{
		unsigned int id = 0;
		pthread_create(&thread_id_, NULL, RecvThread, this);
		printf("start RecvRtmessageMQ thread  id=%d\n", id);
	}

	void RecvRtmessageMQ::Stop()
	{
		pthread_join(thread_id_, NULL);
	}

	void RecvRtmessageMQ::Run()
	{
		if (consumer_)
		{
			consumer_->run();
		}
		printf("run quit!!!\n");
	}

	
}
