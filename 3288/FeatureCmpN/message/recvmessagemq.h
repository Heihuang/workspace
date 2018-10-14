#ifndef RECVMESSAGEMQ_H
#define RECVMESSAGEMQ_H
#include <pthread.h>
#include <map>
#include "../factory/mqfactory.h"
#include "../servicemodules/featurecmp.h"
#include "../servicemodules/featurecmpncao.h"
#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
#include "highgui.h"

class RecvmessageMQ
{
public:
    RecvmessageMQ();
    ~RecvmessageMQ();
    void CreateConsumer();
    void RecvMsg(const std::string& taskid, const std::vector<unsigned char>& fea);
    void setFeatureCmp(const FeatureCmp* feacmp);
    void start();
    void stop();
    static void __stdcall CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user);
    void work();
private:
    static void* run(void* param);
private:
    Consumer* consumer_;
    FeatureCmp* featurecmp_;
    bool thread_flag_;
    pthread_t id_;
};


class ListFeatureUpdate
{
public:
    ListFeatureUpdate();
    ~ListFeatureUpdate();
    void CreateConsumer();
    void RecvMsg(const std::string& blackid, const std::vector<unsigned char>& fea, const std::string& isDelete);
    void setFeatureCmp(const FeatureCmp* feacmp);
    void start();
    void stop();
    static void __stdcall CBMessage(std::map<std::string, std::vector<unsigned char> > msg, void* user);
    void work();
private:
    static void* run(void* param);
private:
    Consumer* consumer_;
    FeatureCmp* featurecmp_;
    bool thread_flag_;
    pthread_t id_;
};
#endif // RECVMESSAGEMQ_H
