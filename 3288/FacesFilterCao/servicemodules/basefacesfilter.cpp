#include "basefacesfilter.h"
#include <stdio.h>

BaseFacesFilter::BaseFacesFilter():sender_(NULL)
{
    sender_ = new SendMessageMQ();
    if(sender_ == NULL)
    {
        printf("new SendMessageMQ failed\n");
    }
}

BaseFacesFilter::~BaseFacesFilter()
{
    if(sender_)
    {
        delete sender_;
        sender_ = NULL;
    }
}
