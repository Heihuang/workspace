#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H
#include <vector>
#include <map>
#include "../factory/mqfactory.h"
#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
#include "../baseutils/mat2img.h"
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
    void RecvData(const std::string& taskid,
                  const std::vector<unsigned char>& fea, const std::string& fea_state);
private:
    std::string task_id_;
    std::vector<unsigned char> fea_;
    std::string fea_state_;
};

#endif // SENDMESSAGE_H
