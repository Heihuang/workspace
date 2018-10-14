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
#include <unistd.h>
#include <memory>
#include <vector>
#include <map>
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
            //debug
            std::string res = message->getString("SearchResult");
            printf("send a res=%s\n", res.c_str());
            producer_->send(static_cast<cms::Message*>(message));
            delete message;
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
        message->setString("TaskId", info.getTaskID());
        message->setIntProperty("TaskId", atoi(info.getTaskID().c_str()));
        printf("taskid=%s\n", info.getTaskID().c_str());
        {
            message->setString("SearchResult", info.getSearchResult());
            message->setString("Score", info.getScore());
            message->setString("BlackID", info.getBlackID());
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
    int task_type_;
private:
    Consumer(const Consumer&);
    Consumer& operator=(const Consumer&);

public:

    Consumer() :
        connection_(NULL),
        session_(NULL),
        destination_(NULL),
        consumer_(NULL),
        cb_msg_(NULL),
        task_type_(0)
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
                if(task_type_ == 1)
                {
                    destination_ = session_->createQueue(qname);
                }
                else if(task_type_ == 2)
                {
                    destination_ = session_->createTopic(qname);
                }
                else
                {}
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
    void SetTaskType(const int& type)
    {
        task_type_ = type;
    }
    // Called from the consumer since this class is a registered MessageListener.
    virtual void run()
    {
        while(true)
        {
            try
            {
                switch (task_type_) {
                case 1:
                    Task1();
                    break;
                case 2:
                    Task2();
                    break;
                default:
                    break;
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
    void Task1()
    {
        const cms::MapMessage* mapMessage = dynamic_cast<const cms::MapMessage*>(consumer_->receive());
        if (mapMessage != NULL)
        {
            std::vector<unsigned char> bytes;
            std::string task;
            std::map<std::string, std::vector<unsigned char> > msg;
            std::vector<std::string> key = mapMessage->getMapNames();
            std::vector<std::string>::iterator it = key.begin();
            while (it != key.end())
            {
               if("TaskId" == *it)
               {
                    task = mapMessage->getString(*it);
                    bytes.assign(task.c_str(), task.c_str() + task.size());
                    msg["TaskId"] = bytes;
                    printf("consumer taskid =%s", task.c_str());
               }
               else if("FeatureValueSize" == *it)
               {
                    std::string temp = mapMessage->getString(*it);
                    bytes.assign(temp.c_str(), temp.c_str() + temp.size());
                    msg["FeatureValueSize"] = bytes;
               }
                else if("FeatureValue" == *it)
                {
                    bytes = mapMessage->getBytes(*it);
                    msg["FeatureValue"] = bytes;
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
    }

    void Task2()
    {
        const cms::MapMessage* mapMessage = dynamic_cast<const cms::MapMessage*>(consumer_->receive());
        if (mapMessage != NULL)
        {
            std::vector<unsigned char> bytes;
            std::string task;
            std::map<std::string, std::vector<unsigned char> > msg;
            std::vector<std::string> key = mapMessage->getMapNames();
            std::vector<std::string>::iterator it = key.begin();
            while (it != key.end())
            {
               std::string temp;
               if("BLACKLIST_ID" == *it)
               {
                    task = mapMessage->getString(*it);
                    bytes.assign(task.c_str(), task.c_str() + task.size());
                    msg["BLACKLIST_ID"] = bytes;
                    printf("consumer taskid =%s\n", task.c_str());
               }
               else if("FeatureValueSize" == *it)
               {
                    temp = mapMessage->getString(*it);
                    bytes.assign(temp.c_str(), temp.c_str() + temp.size());
                    msg["FeatureValueSize"] = bytes;
               }
                else if("FeatureValue" == *it)
                {
                    bytes = mapMessage->getBytes(*it);
                    msg["FeatureValue"] = bytes;
                }
               else if("isDelete" == *it)
               {
                   temp = mapMessage->getString(*it);
                   bytes.assign(temp.c_str(), temp.c_str() + temp.size());
                   msg["isDelete"] = bytes;
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
