#include "recvmessagemq.h"
#include <string>
#include <vector>
#include <stdio.h>
#include "../baseutils/RrConfig.h"
RecvmessageMQ::RecvmessageMQ():consumer_(NULL), thread_flag_(false), featurecmp_(NULL)
{

}

RecvmessageMQ::~RecvmessageMQ()
{
    if(consumer_)
    {
        consumer_->Close();
        MqFactory::getInstance()->DestroyConsumerProc(consumer_);
        printf("connect close!!!\n");
    }
}

void __stdcall RecvmessageMQ::CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user)
{
    RecvmessageMQ* self = static_cast<RecvmessageMQ*>(user);
    if(self)
    {
        std::string taskid;
        std::vector<unsigned char> faceinfo;
        std::map<std::string, std::vector<unsigned char> >::iterator it = msg.begin();
        while (it != msg.end())
        {
            if(it->first == "TaskId")
            {
                taskid.assign(it->second.begin(), it->second.end());
            }
            else if(it->first == "FeatureValueSize")
            {

            }
            else if(it->first == "FeatureValue")
            {
                faceinfo = it->second;
            }
            printf("key=%s\n", it->first.c_str());
            it++;
        }
        printf("consumer start!!!! taskid=%s\n", taskid.c_str());
        if(!faceinfo.empty())
        {   printf("feaceinfo size = %d\n", faceinfo.size() );
            self->RecvMsg(taskid, faceinfo);
        }
        else
        {
            printf("fasceinfo === size=0\n");
        }
    }
}

void RecvmessageMQ::CreateConsumer()
{
    rr::RrConfig config;
    config.ReadConfig("config.ini");
    std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "failover:(tcp://127.0.0.1:61616)");
    std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
    std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
    std::string qname = config.ReadString("ACTIVEMQ", "RecvQueuename", "ComponentTalkQueue");

    consumer_ = MqFactory::getInstance()->CreateConsumerProc();
    if(consumer_)
    {
        consumer_->SetTaskType(1);
        consumer_->Connect(ip, user, password);
        consumer_->Queue(qname);
        consumer_->RegUser(this);
        consumer_->RegCallBack(CBMessage);
        consumer_->CreateRealConsumer();
        printf("connect mq sucessed!!!\n");
//        consumer_->run();
    }
    else
    {
        printf("connect mq failed!!!\n");
    }
}

void RecvmessageMQ::setFeatureCmp(const FeatureCmp *feacmp)
{
    featurecmp_ = const_cast<FeatureCmp*>(feacmp);
}

void RecvmessageMQ::RecvMsg(const std::string& taskid, const std::vector<unsigned char>& fea)
{
    if(featurecmp_)
    {
        featurecmp_->recvMQFeature(fea, taskid);
    }
}

void RecvmessageMQ::start()
{
    thread_flag_ = true;
    pthread_create(&id_, NULL, run, this);
    printf("Recvmessage start\n");
}

void RecvmessageMQ::stop()
{
    thread_flag_ = false;
    pthread_join(id_, NULL);
}

void* RecvmessageMQ::run(void* param)
{
    RecvmessageMQ* self = (RecvmessageMQ*)(param);
    if(self == NULL)
    {
        return NULL;
    }
    self->work();
}

void RecvmessageMQ::work()
{
    if(thread_flag_)
    {
        consumer_->run();
    }
}


ListFeatureUpdate::ListFeatureUpdate():consumer_(NULL), thread_flag_(false), featurecmp_(NULL)
{

}

ListFeatureUpdate::~ListFeatureUpdate()
{
    if(consumer_)
    {
        consumer_->Close();
        MqFactory::getInstance()->DestroyConsumerProc(consumer_);
        printf("connect close!!!\n");
    }
}

void ListFeatureUpdate::CreateConsumer()
{
    rr::RrConfig config;
    config.ReadConfig("config.ini");
    std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "failover:(tcp://127.0.0.1:61616)");
    std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
    std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
    std::string qname = config.ReadString("CENTRALITYMQ", "UpdateQueuename", "ComponentTalkQueue");

    consumer_ = MqFactory::getInstance()->CreateConsumerProc();
    if(consumer_)
    {
        consumer_->SetTaskType(2);
        consumer_->Connect(ip, user, password);
        consumer_->Queue(qname);
        consumer_->RegUser(this);
        consumer_->RegCallBack(CBMessage);
        consumer_->CreateRealConsumer();
        printf("connect mq sucessed!!! ip = %s\n", ip.c_str());
//        consumer_->run();
    }
    else
    {
        printf("connect mq failed!!!\n");
    }
}

void ListFeatureUpdate::RecvMsg(const std::string &blackid, const std::vector<unsigned char> &fea, const std::string& isDelete)
{
    if(featurecmp_)
    {
        if(isDelete == "0")
        {
            featurecmp_->recvUpdateFeature(fea, blackid);
        }
        else if(isDelete == "1")
        {
            featurecmp_->recvDeleteFeature(blackid);
        }
    }
}

void ListFeatureUpdate::setFeatureCmp(const FeatureCmp *feacmp)
{
    featurecmp_ = const_cast<FeatureCmp*>(feacmp);
}
void ListFeatureUpdate::start()
{
    thread_flag_ = true;
    pthread_create(&id_, NULL, run, this);
    printf("ListFeatureUpdate start\n");
}

void ListFeatureUpdate::stop()
{
    thread_flag_ = false;
    pthread_join(id_, NULL);
}

void __stdcall ListFeatureUpdate::CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void *user)
{
    ListFeatureUpdate* self = static_cast<ListFeatureUpdate*>(user);
    if(self)
    {
        std::vector<unsigned char> faceinfo;
        std::string blacklist_id;
        std::string isDelete;
        std::map<std::string, std::vector<unsigned char> >::iterator it = msg.begin();
        while (it != msg.end())
        {
            if(it->first == "BLACKLIST_ID")
            {
                blacklist_id.assign(it->second.begin(), it->second.end());
            }
            else if(it->first == "FeatureValueSize")
            {

            }
            else if(it->first == "FeatureValue")
            {
                faceinfo = it->second;
            }
            else if(it->first == "isDelete")
            {
                isDelete.assign(it->second.begin(), it->second.end());
            }
            printf("key=%s\n", it->first.c_str());
            it++;
        }
        printf("consumer start!!!! BLACKLIST_ID=%s\n", blacklist_id.c_str());
        //if(faceinfo.empty())
        {   printf("feaceinfo size = %d\n", faceinfo.size() );
            self->RecvMsg(blacklist_id, faceinfo, isDelete);
        }

    }
}

void* ListFeatureUpdate::run(void* param)
{
    ListFeatureUpdate* self = (ListFeatureUpdate*)(param);
    if(self == NULL)
    {
        return NULL;
    }
    self->work();
}

void ListFeatureUpdate::work()
{
    if(thread_flag_)
    {
        consumer_->run();
    }
}
