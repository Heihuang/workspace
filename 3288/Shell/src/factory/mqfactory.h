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
#include <vector>
#include <map>
#include <unistd.h>
#include "../message/messageinfo.h"

namespace rr
{
#define  __stdcall
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


class UsbCamaraRegistProducer : public Producer
{
public:
	UsbCamaraRegistProducer()
	{
	}
	virtual ~UsbCamaraRegistProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("DevID", info.getDevId());
		message->setString("IPC_ID", info.getIPCId());
	}
};

class DeviceRegistProducer : public Producer
{
public:
	DeviceRegistProducer()
	{
	}
	virtual ~DeviceRegistProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("DevID", info.getDevId());
		message->setString("DevType", info.getDevType());
		message->setString("DevModel", info.getDevModel());
		message->setString("IP", info.getDevIp());
		message->setString("IPC_ID", info.getIPCId());
	}
};

class DeviceStatusUpdateProducer : public Producer
{
public:
	DeviceStatusUpdateProducer()
	{
	}
	virtual ~DeviceStatusUpdateProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("DevID", info.getDevId());
	}
};

class UsbCamaraStatusUpdateProducer : public Producer
{
public:
	UsbCamaraStatusUpdateProducer()
	{
	}
	virtual ~UsbCamaraStatusUpdateProducer()
	{
	}
	virtual void Task(cms::MapMessage* message, const MessageInfo& info)
	{
		message->setString("IPC_ID", info.getIPCId());
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
			printf("consumer is null\n");
			return;
		}
		while (true)
		{
			try
			{
				printf("running\n");
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
			catch (cms::CMSException& e) 
			{
				e.printStackTrace();
			}
			usleep(10000);
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

class DeviceStatusUpdateRequestConsumer : public Consumer
{
public:
	DeviceStatusUpdateRequestConsumer()
	{
	}
	virtual ~DeviceStatusUpdateRequestConsumer()
	{
	}
	virtual void Task(const cms::MapMessage* mapMessage, std::map<std::string, std::vector<unsigned char> >& msg)
	{
		std::vector<unsigned char> bytes;
		std::vector<std::string> key = mapMessage->getMapNames();
		std::vector<std::string>::iterator it = key.begin();
		while (it != key.end())
		{
			std::string temp;
			if ("DevID" == *it)
			{
				temp = mapMessage->getString(*it);
				bytes.assign(temp.c_str(), temp.c_str() + temp.size());
				msg["DevID"] = bytes;
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

class DeviceRegistProducerFactory : public MqProducerFactory
{
public:
	DeviceRegistProducerFactory();
	virtual ~DeviceRegistProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class UsbCamaraRegistProducerFactory : public MqProducerFactory
{
public:
	UsbCamaraRegistProducerFactory();
	virtual ~UsbCamaraRegistProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class UsbCamaraStatusUpdateProducerFactory : public MqProducerFactory
{
public:
	UsbCamaraStatusUpdateProducerFactory();
	virtual ~UsbCamaraStatusUpdateProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class DeviceStatusUpdateProducerFactory : public MqProducerFactory
{
public:
	DeviceStatusUpdateProducerFactory();
	virtual ~DeviceStatusUpdateProducerFactory();
	virtual void CreateProducerProc(Producer** obj);
	virtual void DestroyProducerProc(Producer* obj);
};

class DeviceStatusUpdateRequestConsumerFactory : public MqConsumerFactory
{
public:
	DeviceStatusUpdateRequestConsumerFactory();
	virtual ~DeviceStatusUpdateRequestConsumerFactory();
	virtual void CreateConsumerProc(Consumer** obj);
	virtual void DestroyConsumerProc(Consumer* obj);
};

}
#endif // MQFACTORY_H
