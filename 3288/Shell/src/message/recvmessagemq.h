#ifndef RECVMESSAGEMQ_H
#define RECVMESSAGEMQ_H
#include <map>
#include <string>
#include <pthread.h>
#include "../factory/mqfactory.h"
#include "../servicemodules/DeviceStatusUpdate.h"

namespace rr
{
	class BaseRecvmessageMQ
	{

	public:
		BaseRecvmessageMQ();
		virtual ~BaseRecvmessageMQ();
		virtual void Run() = 0;
		virtual void RegisterMQ() = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		void setDevUpdate(const DeviceStatusUpdate* devupdate);
	protected:
		Consumer* consumer_;
		MqConsumerFactory* mqfactory_;
		DeviceStatusUpdate* dev_update_;
		std::string dev_id_;
		bool thread_flag_;
		pthread_t thread_id_;
		void CreateConsumer();
		void DestroyConsumer();
	};

	class RecvRtmessageMQ : public BaseRecvmessageMQ
	{
	public:
		RecvRtmessageMQ();
		~RecvRtmessageMQ();
		virtual void Run();
		virtual void RegisterMQ();
		void RecvMsg(const std::string& devid);
		static void __stdcall CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user);
		static void* RecvThread(void* param);
		virtual void Start();
		virtual void Stop();
	};
}
#endif // RECVMESSAGEMQ_H
