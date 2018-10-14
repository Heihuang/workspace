#include "mqfactory.h"
#include <string>
#include <mutex>

MqFactory* MqFactory::singleton_ = NULL;
std::mutex g_locker;
MqFactory* MqFactory::getInstance()
{

    if(singleton_ == NULL)
    {
        g_locker.lock();
        if(singleton_ == NULL)
        {
            singleton_ = new MqFactory;
        }
        g_locker.unlock();
    }
    return singleton_;
}

Producer *MqFactory::CreateProducerProc()
{
    return new Producer;
}

void MqFactory::DestroyProducerProc(Producer *obj)
{
    if(obj)
    {
        obj->Close();
        delete obj;
    }
}

Consumer *MqFactory::CreateConsumerProc()
{
    return new Consumer;
}

void MqFactory::DestroyConsumerProc(Consumer *obj)
{
    if(obj)
    {
        obj->Close();
        delete obj;
    }
}
