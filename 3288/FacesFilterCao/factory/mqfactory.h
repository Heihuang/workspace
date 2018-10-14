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
	
	 void Topic(const std::string & qname)
    {
        try
        {
            //OutputLog_Info("Debug");
            // Create a Session
            if (connection_)
            {
                session_ = connection_->createSession(cms::Session::AUTO_ACKNOWLEDGE);
            }
            // Create the destination (Topic or Queue)
            if (session_)
            {
                //destination_ = session_->createQueue(qname);
                destination_ = session_->createTopic(qname);
                printf("Topic name=%s", qname.c_str());
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
            //std::string str = message->getString("TaskId");
            //print("send taskid=%s", str.c_str());
           // qDebug() << str.c_str();
            producer_->send(static_cast<cms::Message*>(message));
            delete message;
	    message = NULL;
        }
        catch (cms::CMSException& e)
        {
            e.printStackTrace();
        }
    }

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

    void Task(cms::MapMessage* message, const MessageInfo& info )
    {
		message->setIntProperty("TaskId", atoi(info.getTaskID().c_str()));
        message->setString("TaskId", info.getTaskID());
        if("string" == info.getDataType())
        {
           // message->setString("MessageData", info.getMsgData());
        }
       // else if("blob" == info.getDataType() && !info.getBlobData().empty())
        {
           // message->setString("MessageData", info.getMsgData());
           //cmd message->setString("FeatureValueSize", info.getBlobDataSize());
            message->setBytes("FeatureValue", info.getBlobData());
            message->setString("FilterFlag", info.getMsgData());
           // message->setString("FaceInfoSize", info.getBlobFaceInfoSize());
           // message->setBytes("FaceInfo", info.getBlobFaceInfo());
        }
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

    void Topic(const std::string & qname)
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
        if(consumer_ == NULL)
        {
            printf("consumer is null\n");
        }
        while(true)
        {
            try
            {
                const cms::MapMessage* mapMessage = dynamic_cast<const cms::MapMessage*>(consumer_->receive());
                //OutputLog_Error("debug!!!");
                if (mapMessage != NULL)
                {
                    std::vector<unsigned char> bytes;
                    std::string task;
                    std::map<std::string, std::vector<unsigned char> > msg;
                    std::vector<std::string> key = mapMessage->getMapNames();
                    std::vector<std::string>::iterator it = key.begin();
                    while (it != key.end())
                    {
                        if("MessageData" == *it)
                        {
                            std::string temp = mapMessage->getString(*it);
                            bytes.assign(temp.c_str(), temp.c_str() + temp.size());
                            msg["MessageData"] = bytes;
                        }
                        else if("BlobData" == *it)
                        {
                            bytes = mapMessage->getBytes(*it);
                            msg["BlobData"] = bytes;
                        }
                        else if("TaskId" == *it)
                        {
                            task = mapMessage->getString(*it);
                            bytes.assign(task.c_str(), task.c_str() + task.size());
                            msg["TaskId"] = bytes;
                        }
                        else if("BlobdataSize" == *it)
                        {
                            std::string temp = mapMessage->getString(*it);
                            bytes.assign(temp.c_str(), temp.c_str() + temp.size());
                            msg["BlobdataSize"] = bytes;
                        }
                        else if("FaceInfoSize" == *it)
                        {
                            std::string temp = mapMessage->getString(*it);
                            bytes.assign(temp.c_str(), temp.c_str() + temp.size());
                            msg["FaceInfoSize"] = bytes;
                        }
                        else if("FaceInfo" == *it)
                        {
                            bytes = mapMessage->getBytes(*it);
                            msg["FaceInfo"] = bytes;
                        }
                        //
                        it++;
                    }
                    if (cb_msg_)
                    {
                        cb_msg_(msg, user_);
                    }
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
            usleep(10000);
        }

    }

private:
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
            if (consumer_!= NULL)
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

class MqFactory
{
public:
    ~MqFactory()
    {
        activemq::library::ActiveMQCPP::shutdownLibrary();
    }
    static MqFactory* getInstance();
//    {

//        if(singleton_ == NULL)
//        {
//           // g_locker.lock();
//            if(singleton_ == NULL)
//            {
//                singleton_ = new MqFactory;
//            }
//          //  g_locker.unlock();
//        }
//        return singleton_;
//    }

    Producer *CreateProducerProc();
    void DestroyProducerProc(Producer *obj);
    Consumer *CreateConsumerProc();
    void DestroyConsumerProc(Consumer* obj);
private:
    MqFactory()
    {
       // OutputLog_Error("MqFactory");
        activemq::library::ActiveMQCPP::initializeLibrary();
    }
    MqFactory(const MqFactory&);
    MqFactory& operator=(const MqFactory&);
    static MqFactory* singleton_;
};


#endif // MQFACTORY_H
