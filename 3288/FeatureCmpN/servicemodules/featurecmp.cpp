#include "featurecmp.h"

FeatureCmp::FeatureCmp() : sender_(NULL), score_(0.0), srcfea_flag_(false)
{
    sender_ = new SendMessageMQ();
    if(sender_ == NULL)
    {
        return;
    }
}

FeatureCmp::~FeatureCmp()
{
    srcfea_flag_ = false;
    if(sender_)
    {
        delete sender_;
        sender_ = NULL;
    }
}


