#ifndef USB_STATUS_UPDATE_H_
#define USB_STATUS_UPDATE_H_
#include <string>
#include <pthread.h>
#include "../message/sendmessageMQ.h"

namespace rr
{

	class UsbUpdate
	{
	public:
		UsbUpdate();
		~UsbUpdate();
		void Update();
		void SendAliveMsg();
		void setIPCId(const std::string& ipcid);
		static void* SendThread(void* param);
		void Start();
		void Stop();
	private:
		pthread_t thread_id_;
		std::string ipc_id_;
		bool flag_;
		SendUsbStatusUpdateMessageMQ *sender_;
	};

}
#endif