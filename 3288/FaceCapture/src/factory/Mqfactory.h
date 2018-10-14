#ifndef MQFACTORY_H
#define MQFACTORY_H
#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>
#include <map>
#include "../message/Messageinfo.h"
#include "../../logprocessor.h"
namespace rr
{
#define __stdcall

typedef void(__stdcall *CallBackMsg)(std::map<std::string, std::vector<unsigned char> > msg, void* user);

class Producer : public decaf::lang::Runnable
{
public:
	Producer() : connection_(NULL), session_(NULL), destination_(NULL), producer_(NULL)
	{

	}

	~Producer()
	{
		Cleanup();
	}

	virtual void run()
	{
		try
		{


		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void Connect(const std::string& url, const std::string&username, const std::string& password)
	{
		try
		{
			// Create a ConnectionFactory
			std::auto_ptr<cms::ConnectionFactory> connectionFactory(cms::ConnectionFactory::createCMSConnectionFactory(url));
			// Create a Connection
			connection_ = connectionFactory->createConnection(username, password);
			//connection_ = connectionFactory->createConnection();
			if (connection_)
			{
				connection_->start();
				printf("debug!!!!");
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void Queue(const std::string & qname)
	{
		try
		{
			// Create a Session
			if (connection_)
			{
				session_ = connection_->createSession(cms::Session::AUTO_ACKNOWLEDGE);
			}
			// Create the destination (Topic or Queue)
			if (session_)
			{
				destination_ = session_->createQueue(qname);
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void Topic(const std::string& qname)
	{
		try
		{
			// Create a Session
			if (connection_)
			{
				session_ = connection_->createSession(cms::Session::AUTO_ACKNOWLEDGE);
			}
			// Create the destination (Topic or Queue)
			if (session_)
			{
				destination_ = session_->createTopic(qname);
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void CreateRealProducer()
	{
		try
		{
			// Create a MessageProducer from the Session to the Topic or Queue
			producer_ = session_->createProducer(destination_);
			producer_->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void SendMsg(const MessageInfo &info)
	{
		try
		{
			//调用producer发送消息
			cms::MapMessage *message = session_->createMapMessage();
			Task(message, info);
			producer_->send(static_cast<cms::Message*>(message));
			delete message;
			message = NULL;
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info) = 0;
	void Close()
	{
		Cleanup();
	}

private:
	cms::Connection* connection_;
	cms::Session* session_;
	cms::Destination* destination_;
	cms::MessageProducer* producer_;
private:
	Producer(const Producer& other);
	Producer& operator=(const Producer& other);
	void Cleanup()
	{

		if (connection_ != NULL) {
			try {
				connection_->close();
			}
			catch (cms::CMSException& ex) {
				ex.printStackTrace();
			}
		}

		// Destroy resources.
		try
		{
			if (destination_ != NULL)
			{
				delete destination_;
				destination_ = NULL;
			}
			if (producer_ != NULL)
			{
				delete producer_;
				producer_ = NULL;
			}
			if (session_ != NULL)
			{
				delete session_;
				session_ = NULL;
			}
			if (connection_ != NULL)
			{
				delete connection_;
				connection_ = NULL;
			}
		}
		catch (cms::CMSException& e) {
			e.printStackTrace();
		}
	}
};


class DetectorProducer : public Producer
{
public:
	DetectorProducer()
	{
	}
	virtual ~DetectorProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("IPC_ID", info.getIpcId());
		message->setBytes("BlobData", info.getBlobData());
		message->setString("CurTime", info.getCurTime());
		message->setString("FaceScore", info.getFaceScore());
		message->setString("FaceYaw", info.getYaw());
		message->setString("FacePitch", info.getPitch());
		message->setString("FaceRoll", info.getRoll());
		message->setString("FaceId", info.getFaceId());
		message->setString("EyeWidth", info.getEyeWidth());
		message->setString("FaceWidth", info.getFaceWidth());
		message->setString("Definition", info.getDefinition());
		message->setString("Brightness", info.getBrightness());
		message->setString("DevID", info.getDevID());
		message->setString("FaceQualityScore", info.getFaceQualityScore());
		message->setString("FaceSize", info.getFaceSize());
		message->setBytes("FullView", info.getFullViewData());
		message->setString("TalkId",info.getTalkId());
		printf("TalkID=%s\n", info.getTalkId().c_str());
	}
};

class FullViewProducer : public Producer
{
public:
	FullViewProducer()
	{
	}
	virtual ~FullViewProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("IPC_ID", info.getIpcId());
		message->setString("DevID", info.getDevID());
		message->setBytes("BlobData", info.getBlobData());
		message->setString("CurTime", info.getCurTime());
		message->setString("QueueName","VideoPreview");
	}
};

class GetIpcIdProducer : public Producer
{
public:
	GetIpcIdProducer()
	{
	}
	virtual ~GetIpcIdProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("DevID", info.getDevID());
	}
};

class FaceUpdateProducer : public Producer
{
public:
	FaceUpdateProducer()
	{
	}
	virtual ~FaceUpdateProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("DevID", info.getDevID());
		message->setString("IPC_ID", info.getIpcId());
		message->setString("FaceId", info.getFaceId());
		message->setString("CurTime", info.getCurTime());
		message->setString("QueueName", "UpdateFaceInfo");
	}
};

class Consumer : public decaf::lang::Runnable
{

private:

	cms::Connection* connection_;
	cms::Session* session_;
	cms::Destination* destination_;
	cms::MessageConsumer* consumer_;
	CallBackMsg cb_msg_;
	void* user_;
private:
	Consumer(const Consumer&);
	Consumer& operator=(const Consumer&);

public:

	Consumer() :
		connection_(NULL),
		session_(NULL),
		destination_(NULL),
		consumer_(NULL),
		cb_msg_(NULL)
	{

	}

	virtual ~Consumer() {
		cleanup();
	}

	void Close() {
		this->cleanup();
	}

	void Connect(const std::string& url, const std::string&username, const std::string& password)
	{
		try
		{
			// Create a ConnectionFactory
			std::auto_ptr<cms::ConnectionFactory> connectionFactory(cms::ConnectionFactory::createCMSConnectionFactory(url));
			// Create a Connection
			connection_ = connectionFactory->createConnection(username, password);
			if (connection_)
			{
				connection_->start();

			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void Queue(const std::string & qname)
	{
		try
		{
			// Create a Session
			if (connection_)
			{
				session_ = connection_->createSession(cms::Session::AUTO_ACKNOWLEDGE);
			}
			if (session_)
			{
				destination_ = session_->createQueue(qname);
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void Topic(const std::string& qname)
	{
		try
		{
			// Create a Session
			if (connection_)
			{
				session_ = connection_->createSession(cms::Session::AUTO_ACKNOWLEDGE);
			}
			if (session_)
			{
				destination_ = session_->createTopic(qname);
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void CreateRealConsumer(const std::string& consumer_name)
	{
		try
		{
			// Create a MessageConsumer from the Session to the Topic or Queue
			if (session_)
			{
				consumer_ = session_->createConsumer(destination_, consumer_name/*"ClientCode='0008'"*/);
				//consumer_ = session_->createConsumer(destination_);
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void CreateRealConsumer()
	{
		try
		{
			// Create a MessageConsumer from the Session to the Topic or Queue
			if (session_)
			{
				consumer_ = session_->createConsumer(destination_);
			}
			if (consumer_)
			{
				// consumer_->setMessageListener(this);
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}

	void RegCallBack(CallBackMsg cb)
	{
		cb_msg_ = cb;
	}

	void RegUser(void* user)
	{
		user_ = user;
	}

	virtual void run()
	{
		if (consumer_ == NULL)
		{
			return;
		}
		while (true)
		{
			try
			{
				const cms::MapMessage* mapMessage = dynamic_cast<const cms::MapMessage*>(consumer_->receive());
				if (mapMessage != NULL)
				{
					std::map<std::string, std::vector<unsigned char> > msg;
					Task(mapMessage, msg);
					CalledMsg(msg);
					delete mapMessage;
					mapMessage = NULL;
				}
				else
				{
				}
			}
			catch (cms::CMSException& e) {
				e.printStackTrace();
			}
			usleep(1000);
		}

	}
	virtual void Task(const cms::MapMessage* mapMessage, std::map<std::string, std::vector<unsigned char> >& msg) = 0;
private:
	void CalledMsg(const std::map<std::string, std::vector<unsigned char> >& msg)
	{
		if (cb_msg_)
		{
			cb_msg_(msg, user_);
		}
	}
	void cleanup()
	{
		if (connection_ != NULL)
		{
			try
			{
				connection_->close();
			}
			catch (cms::CMSException& ex)
			{
				ex.printStackTrace();
			}
		}
		// Destroy resources.
		try
		{
			if (destination_ != NULL)
			{
				delete destination_;
				destination_ = NULL;
			}
			if (consumer_ != NULL)
			{
				delete consumer_;
				consumer_ = NULL;
			}
			if (session_ != NULL)
			{
				delete session_;
				session_ = NULL;
			}
			if (connection_ != NULL)
			{
				delete connection_;
				connection_ = NULL;
			}
		}
		catch (cms::CMSException& e)
		{
			e.printStackTrace();
		}
	}
};

class FullViewNotifyConsumer : public Consumer
{
public:
	FullViewNotifyConsumer()
	{
	}
	virtual ~FullViewNotifyConsumer()
	{
	}
	virtual void Task(const cms::MapMessage* mapMessage, std::map<std::string, std::vector<unsigned char> >& msg)
	{
		std::vector<unsigned char> bytes;
		std::string task;
		std::vector<std::string> key = mapMessage->getMapNames();
		std::vector<std::string>::iterator it = key.begin();
		while (it != key.end())
		{
			
			//
			if ("IsSend" == *it)
			{

				std::string temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["IsSend"] = bytes;
			}
			else if ("IPC_ID" == *it)
			{
				std::string temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["IPC_ID"] = bytes;
			}
			else if ("DevID" == *it)
			{
				std::string temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["DevID"] = bytes;
			}

			it++;
		}
	}

};

class UpdateIpcConsumer : public Consumer
{
public:
	UpdateIpcConsumer(){}
	virtual ~UpdateIpcConsumer(){}
	virtual void Task(const cms::MapMessage* mapMessage, std::map<std::string, std::vector<unsigned char> >& msg)
	{
		std::vector<unsigned char> bytes;
		std::vector<std::string> key = mapMessage->getMapNames();
		std::vector<std::string>::iterator it = key.begin();
		while (it != key.end())
		{
			std::string temp;
			if ("IPC_ID" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["IPC_ID"] = bytes;
			}
			else if ("IpcUrl" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["IpcUrl"] = bytes;
			}
			else if ("IpcIp" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["IpcIp"] = bytes;
			}
			else if ("IpcPort" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["IpcPort"] = bytes;
			}
			else if ("DevID" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["DevID"] = bytes;
			}
			else if ("isDelete" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["isDelete"] = bytes;
			}
			else if ("IpcNO" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["IpcNO"] = bytes;
			}
			//
			it++;
		}
	}
};

class MqProcessor
{
public:
	MqProcessor()
	{
		// OutputLog_Error("MqFactory");
		activemq::library::ActiveMQCPP::initializeLibrary();
	}
	~MqProcessor()
	{
		activemq::library::ActiveMQCPP::shutdownLibrary();
		printf("free mq lib\n");
	}
private:
	MqProcessor(const MqProcessor&);
	MqProcessor& operator=(const MqProcessor&);
};

class MqConsumerFactory
{
public:
	MqConsumerFactory(){}
	virtual ~MqConsumerFactory(){}
	virtual void CreateConsumerProc(Consumer** obj) = 0;
	virtual void DestroyConsumerProc(Consumer* obj) = 0;
};

class MqProducerFactory
{
public:
	MqProducerFactory(){}
	virtual ~MqProducerFactory(){}
	virtual void CreateProducerProc(Producer** obj) = 0;
	virtual void DestroyProducerProc(Producer* obj) = 0;
};

class DetectorProducerFactory : public MqProducerFactory
{
public:
	DetectorProducerFactory();
	virtual ~DetectorProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class FullViewProducerFactory : public MqProducerFactory
{
public:
	FullViewProducerFactory();
	virtual ~FullViewProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class GetIpcIdProducerFactory : public MqProducerFactory
{
public:
	GetIpcIdProducerFactory();
	virtual ~GetIpcIdProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class FaceUpdateProducerFactory : public MqProducerFactory
{
public:
	FaceUpdateProducerFactory();
	virtual ~FaceUpdateProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class FullViewNotifyConsumerFactory : public MqConsumerFactory
{
public:
	FullViewNotifyConsumerFactory();
	virtual ~FullViewNotifyConsumerFactory();
	virtual void CreateConsumerProc(Consumer** obj);
	virtual void DestroyConsumerProc(Consumer* obj);
};

class UpdateIpcConsumerFactory : public MqConsumerFactory
{
public:
	UpdateIpcConsumerFactory();
	virtual ~UpdateIpcConsumerFactory();
	virtual void CreateConsumerProc(Consumer** obj);
	virtual void DestroyConsumerProc(Consumer* obj);
};
}
#endif // MQFACTORY_H
