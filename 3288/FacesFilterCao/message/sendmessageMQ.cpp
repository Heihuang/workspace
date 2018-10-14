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
        producer_->Topic(qname);
        producer_->CreateRealProducer();
        printf("connect mq succssed!!!!\n");
    }
    else
    {
        //OutputLog_Error("connect mq failed!!!!");
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
    msg.setBlobData(fea_);
    msg.setDataType("blob");
    msg.setTaskID(task_id_);
    msg.setMsgData(fea_state_);
}

void SendMessageMQ::RecvData(const std::string &taskid, const std::vector<unsigned char> &fea,
                             const std::string &fea_state)
{
   task_id_ = taskid;
   fea_ = fea;
   fea_state_ = fea_state;
}
