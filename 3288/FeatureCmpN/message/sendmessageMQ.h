#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include <vector>
#include <map>
#include "../factory/mqfactory.h"
#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
#include "messageinfo.h"

class BaseSendMessageMQ
{
public:
    BaseSendMessageMQ();
    virtual ~BaseSendMessageMQ();
    virtual void CreateMsg(MessageInfo& msg) = 0;
    bool SendMsg();
private:
    Producer* producer_;
};

class SendMessageMQ : public BaseSendMessageMQ
{
public:
    SendMessageMQ();
    ~SendMessageMQ();
    virtual void CreateMsg(MessageInfo& msg);
    void RecvData(const std::string& taskid,const std::string& result, const int& blackid,
                  const float& score);
private:
    int black_id_;
    float score_;
    std::string task_ID_;
    std::string result_;
};
#endif // SENDMESSAGE_H
