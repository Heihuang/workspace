#include "sendmessageMQ.h"
#include <string>
#include <vector>
#include <stdio.h>
#include "../baseutils/RrConfig.h"

#define MAX_LIST 200

BaseSendMessageMQ::BaseSendMessageMQ() : producer_(NULL)
{
    rr::RrConfig config;
    config.ReadConfig("config.ini");
    std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "failover:(tcp://127.0.0.1:61616)");
    std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
    std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
    std::string qname = config.ReadString("ACTIVEMQ", "SendQueuename", "ComponentTalkQueue");
    producer_ = MqFactory::getInstance()->CreateProducerProc();
    if(producer_)
    {
        printf("ip=%s\n", ip.c_str());
        producer_->Connect(ip, user, password);
        //producer_->Queue(qname);
        producer_->Topic(qname);
        producer_->CreateRealProducer();
        printf("connect mq succssed!!!!\n");
    }
    else
    {
        printf("connect mq failed!!!!\n");
        return ;
    }
}

BaseSendMessageMQ::~BaseSendMessageMQ()
{
    if(producer_)
    {
        producer_->Close();
        MqFactory::getInstance()->DestroyProducerProc(producer_);
    }
}


bool BaseSendMessageMQ::SendMsg()
{
    if(producer_)
    {
        MessageInfo msg;
        CreateMsg(msg);
        producer_->SendMsg(msg);
        return true;
    }
    else
    {
        return false;
    }
}

SendMessageMQ::SendMessageMQ()
{

}

SendMessageMQ::~SendMessageMQ()
{

}

void SendMessageMQ::CreateMsg(MessageInfo &msg)
{
    std::string infoid;
    std::string infoscore;
    char buf[255] = {0};
    sprintf(buf, "%d", black_id_);
    if(strlen(buf) == 0)
    {
        return ;
    }
    infoid.assign(buf);
    memset(buf, 0, 255);
    sprintf(buf, "%f", score_);
    if(strlen(buf) == 0)
    {
        return ;
    }
    infoscore.assign(buf);
    printf("id=%s  score=%s\n", infoid.c_str(), infoscore.c_str());
    msg.setBlackID(infoid);
    msg.setScore(infoscore);
    msg.setTaskID(task_ID_);
    msg.setSearchResult(result_);
}

void SendMessageMQ::RecvData(const std::string &taskid, const std::string &result, const int &blackid, const float &score)
{
    black_id_ = blackid;
    score_ = score;
    task_ID_ = taskid;
    result_ = result;
}
