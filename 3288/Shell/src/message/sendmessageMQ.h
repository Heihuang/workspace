#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H
#include <vector>
#include <map>
#include "../factory/mqfactory.h"
#include "messageinfo.h"

namespace rr
{

	class BaseSendMessageMQ
	{
	public:
		BaseSendMessageMQ();
		virtual ~BaseSendMessageMQ();
		virtual void CreateMsg(MessageInfo& msg) = 0;
		bool SendMsg();
	protected:
		Producer* producer_;
		MqProducerFactory* mqfactory_;
	};

	class SendDeviceRegistMessageMQ : public BaseSendMessageMQ
	{
	public:
		SendDeviceRegistMessageMQ();
		virtual ~SendDeviceRegistMessageMQ();
		virtual void CreateMsg(MessageInfo& msg);
		void RecvData(const std::string& devid, const std::string& devtype, const std::string& devmodel, const std::string& devip, const std::string& ipcid);
	private:
		std::string devid_;
		std::string devtype_;
		std::string devmodel_;
		std::string devip_;
		std::string ipcid_;
	};

	class SendUsbCamaraRegistMessageMQ : public BaseSendMessageMQ
	{
	public:
		SendUsbCamaraRegistMessageMQ();
		virtual ~SendUsbCamaraRegistMessageMQ();
		virtual void CreateMsg(MessageInfo& msg);
		void RecvData(const std::string& ipcid, const std::string&devid);
	private:
		std::string ipc_id_;
		std::string dev_id_;
	};

	class SendDeviceStatusUpdateMessageMQ : public BaseSendMessageMQ
	{
	public:
		SendDeviceStatusUpdateMessageMQ();
		virtual ~SendDeviceStatusUpdateMessageMQ();
		virtual void CreateMsg(MessageInfo& msg);
		void RecvData(const std::string&devid);
	private:
		std::string dev_id_;
	};

	class SendUsbStatusUpdateMessageMQ : public BaseSendMessageMQ
	{
	public:
		SendUsbStatusUpdateMessageMQ();
		virtual ~SendUsbStatusUpdateMessageMQ();
		virtual void CreateMsg(MessageInfo& msg);
		void RecvData(const std::string&ipcid);
	private:
		std::string ipc_id_;
	};
}
#endif // SENDMESSAGE_H
