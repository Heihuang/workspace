#ifndef FEATURECMPNCAO_H
#define FEATURECMPNCAO_H

#include <vector>
#include <map>
#include <string>
#include <mutex>
#include "../message/sendmessageMQ.h"
#include "featurecmp.h"
#include "fdc/rr_common.h"
#include "fdc/rr_faceverify.h"

class FeatureCmpNCao : public FeatureCmp
{
public:
    FeatureCmpNCao();
    virtual ~FeatureCmpNCao();
    virtual void recvMQFeature(const std::vector<unsigned char>& mqfea, const std::string& taskid);
    virtual void recvDBFeature(const std::map<int, std::vector<unsigned char> >& dbfea);
    virtual void recvDBScore(const float & score);
    virtual void recvUpdateFeature(const std::vector<unsigned char>& mqfea, const std::string & black_id);
    virtual void recvDeleteFeature(const std::string & black_id);
private:
	std::map<int, rr_feature_t> fea_cao_;
    std::mutex  facedb_locker_;
private:
	void serchFace(rr_feature_t fea, const std::string& task_id);
	float Calcsimilarity(rr_feature_t feat_src, rr_feature_t feat_dst);
    void AddNewFeature(const std::vector<unsigned char>& fea, const std::string &black_id);
    void UpDateFeature(const std::vector<unsigned char>& fea, const std::string &black_id);
};
#endif // FEATURECMPNCAO_H
