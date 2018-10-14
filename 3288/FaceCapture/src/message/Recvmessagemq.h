#ifndef RECVMESSAGEMQ_H
#define RECVMESSAGEMQ_H
#include <map>
#include <pthread.h>
#include <semaphore.h>
#include "../factory/Mqfactory.h"
#include "../servicemodules/Camera.h"
#include "../servicemodules/UsbCamera.h"
#include "../servicemodules/IPCamera.h"
#include "../servicemodules/ManageCamera.h"
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
	void setDevId(const std::string& devid);
protected:
	Consumer* consumer_;
	MqConsumerFactory* mqfactory_;
	bool thread_flag_;
	pthread_t id_;
	void CreateConsumer();
	void DestroyConsumer();
	std::string dev_id_;
};

class FullViewNotifyMQ : public BaseRecvmessageMQ
{
public:
	FullViewNotifyMQ();
	virtual ~FullViewNotifyMQ();
	virtual void Run();
	virtual void RegisterMQ();
	void RecvMsg(const std::string& devid, const std::string& ipcid, const std::string& IsSend);
	static void __stdcall CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user);
	static void* RecvThread(void* param);
	virtual void Start();
	virtual void Stop();
	void setManageCamera(ManageCamera* manage_cm);
private:
	ManageCamera* manage_cm_;
};

class IpcCameraNotifyMQ : public BaseRecvmessageMQ
{
public:
	IpcCameraNotifyMQ();
	virtual ~IpcCameraNotifyMQ();
	virtual void Run();
	virtual void RegisterMQ();
	void RecvMsg(const std::string& devid, const std::string& ipcid, const std::string& ipc_url, const std::string& ipc_ip, const int& ipc_port, const std::string&ipc_No, const int& isdel);
	static void __stdcall CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user);
	static void* RecvThread(void* param);
	virtual void Start();
	virtual void Stop();
	void setManageCamera(ManageCamera* manage_cm);
private:
	ManageCamera* manage_cm_;
};
}
#endif // RECVMESSAGEMQ_H
