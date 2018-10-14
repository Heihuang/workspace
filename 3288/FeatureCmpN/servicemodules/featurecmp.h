#ifndef FEATURECMP_H
#define FEATURECMP_H

#include <vector>
#include <map>
#include <string>
#include "../message/sendmessageMQ.h"

class FeatureCmp
{
public:
    FeatureCmp();
    virtual ~FeatureCmp();
    virtual void recvMQFeature(const std::vector<unsigned char>& mqfea, const std::string& taskid) = 0;
    virtual void recvDBFeature(const std::map<int, std::vector<unsigned char> >& dbfea) = 0;
    virtual void recvDBScore(const float & score) = 0;
    virtual void recvUpdateFeature(const std::vector<unsigned char>& mqfea, const std::string & black_id) = 0;
    virtual void recvDeleteFeature(const std::string & black_id) = 0;
protected:
    float score_;
    bool  srcfea_flag_;
    SendMessageMQ *sender_;
};
#endif // FEATURECMP_H
