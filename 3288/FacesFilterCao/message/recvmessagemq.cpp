#include "recvmessagemq.h"
#include <string>
#include <vector>
#include <stdio.h>
#include "../baseutils/RrConfig.h"
#include "../baseutils/mat2img.h"
#include "../servicemodules/FactoryAlgorithmCao.h"

void __stdcall CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user)
{
    RecvmessageMQ* self = static_cast<RecvmessageMQ*>(user);
    if(self)
    {
        std::string taskid;
        std::vector<unsigned char> img;
        std::vector<unsigned char> faceinfo;
        std::string messagedata;
      //  int ipc = 1;
        std::map<std::string, std::vector<unsigned char> >::iterator  it = msg.begin();
        while (it != msg.end())
        {
            if(it->first == "BlobData")
            {
                img = it->second;
            }
            else if(it->first == "BlobDataSize")
            {

            }
            else if(it->first == "TaskId")
            {
                taskid.assign(it->second.begin(), it->second.end());
            }
            else if(it->first == "FaceInfoSize")
            {

            }
            else if(it->first == "FaceInfo")
            {
                faceinfo = it->second;
            }
            else if(it->first == "MessageData")
            {
                messagedata.assign(it->second.begin(), it->second.end());
            }
            it++;
        }
        if(!img.empty())
        {
            self->RecvMsg(taskid, img);
        }
        else
        {
        }
    }
}

RecvmessageMQ::RecvmessageMQ():consumer_(NULL), face_filter_(NULL), algorithm_(NULL)
{
    algorithm_ = new FactoryAlgorithmCao();
    if(algorithm_ == NULL)
    {
        return;
    }
    algorithm_->CreateFaceVerify(&face_filter_);
    if(face_filter_ == NULL)
    {
        return;
    }

}

RecvmessageMQ::~RecvmessageMQ()
{
    if(consumer_)
    {
        consumer_->Close();
        MqFactory::getInstance()->DestroyConsumerProc(consumer_);
    }
    if(algorithm_ && face_filter_)
    {
        algorithm_->DestroyFaceVerify(face_filter_);
        delete algorithm_;
        algorithm_ = NULL;
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
    printf("CreateConsumer\n");
    if(consumer_)
    {
        consumer_->Connect(ip, user, password);
        consumer_->Queue(qname);
        consumer_->RegUser(this);
        consumer_->RegCallBack(CBMessage);
        consumer_->CreateRealConsumer();
        printf("connect mq sucessed!!! ip=%s\n", ip.c_str());
        consumer_->run();
    }
    else
    {
        //OutputLog_Error("connect mq failed!!!");
    }
}

void RecvmessageMQ::RecvMsg(const std::string &taskid, const std::vector<unsigned char> &img)
{
    if(img.empty())
    {	
        return;
    }
    Mat2Img mat2img;
    cv::Mat mat;
    mat2img.JPEGToMat(mat, img);
    //filte face
    if(face_filter_)
    {
        face_filter_->FilterFace(mat, taskid);
    }
}
