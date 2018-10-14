#include "featurecmpncao.h"
#include "../globalcommon.h"
#include <stdio.h>
#include <math.h>
#define FEATURES_LEN 512

FeatureCmpNCao::FeatureCmpNCao()
{

}

FeatureCmpNCao::~FeatureCmpNCao()
{
    facedb_locker_.lock();
    fea_cao_.clear();
    facedb_locker_.unlock();
}

void FeatureCmpNCao::recvMQFeature(const std::vector<unsigned char> &mqfea, const std::string &taskid)
{
    printf("rt fea size=%d\n", mqfea.size());
	rr_feature_t fea;
	memcpy(&fea, mqfea.data(), mqfea.size());
	serchFace(fea, taskid);
}

void FeatureCmpNCao::recvDBFeature(const std::map<int, std::vector<unsigned char> > &dbfea)
{
    if(dbfea.empty())
    {
        return;
    }
    std::map<int, std::vector<unsigned char> >::const_iterator it = dbfea.begin();
    while(it != dbfea.end())
    {
        std::vector<unsigned char> tmp = it->second;
		rr_feature_t fea;
		memcpy(&fea, tmp.data(), tmp.size());
		fea_cao_.insert(std::make_pair(it->first, fea));
		printf("Add blackfea, blackid=%d\n", it->first);
        ++it;
    }
}

void FeatureCmpNCao::recvDBScore(const float &score)
{
    score_ = score;
    printf("threshold = %f\n", score);
}

void FeatureCmpNCao::serchFace(rr_feature_t fea, const std::string &task_id)
{
    std::map<float, int> serch_map;
    std::map<float, int> min_map;
    float sc = 0.0;
    int Id = 0;
	std::map<int, rr_feature_t>::iterator it = fea_cao_.begin();
    while(it != fea_cao_.end())
    {
		rr_feature_t fea_src = it->second;
        sc = Calcsimilarity(fea_src, fea);
        if(sc > score_)
        {
            serch_map.insert(std::make_pair(sc, it->first));
        }
        else
        {
            min_map.insert(std::make_pair(sc, it->first));
        }
        it++;
    }
    ST_RESULT result;
    if(!serch_map.empty())
    {
       std::map<float, int>::iterator it_rt = serch_map.end();
       it_rt--;
       sc = it_rt->first;
       Id = it_rt->second;

       printf("searchFace face > score= %f\n", sc);
       result.id = Id;
       result.score = sc;
       result.res = "true";
       result.task_ID = task_id;
       if(sender_)
       {
           sender_->RecvData(result.task_ID, result.res, result.id, result.score);
           sender_->SendMsg();
       }
    }
    else
    {
        if(!min_map.empty())
        {
            std::map<float, int>::iterator it_min = min_map.end();
            it_min--;
            sc = it_min->first;
            Id = it_min->second;
            result.id = Id;
            result.score = sc;
            result.res = "false";
            result.task_ID = task_id;
            if(sender_)
            {
                sender_->RecvData(result.task_ID, result.res, result.id, result.score);
                sender_->SendMsg();
            }
        }
        else
        {
            result.id = Id;
            result.score = sc;
            result.res = "false";
            result.task_ID = task_id;
            if(sender_)
            {
                sender_->RecvData(result.task_ID, result.res, result.id, result.score);
                sender_->SendMsg();
            }
        }
    }
}


float FeatureCmpNCao::Calcsimilarity(rr_feature_t feat_src, rr_feature_t feat_dst)
{
	float similarity = rr_fv_compare_features(&feat_src, &feat_dst);
    return similarity;
}

void FeatureCmpNCao::recvUpdateFeature(const std::vector<unsigned char> &mqfea, const std::string &black_id)
{
    unsigned char data[FEATURES_LEN * 4];
    facedb_locker_.lock();
	std::map<int, rr_feature_t>::iterator it = fea_cao_.find(atoi(black_id.c_str()));
    if(it != fea_cao_.end())
    {
        fea_cao_.erase(it);
		rr_feature_t fea;
		memcpy(&fea, mqfea.data(), mqfea.size());
		fea_cao_.insert(std::make_pair(atoi(black_id.c_str()), fea));
		printf("update black_id=%s\n", black_id.c_str());
    }
    else
    {
		rr_feature_t fea;
		memcpy(&fea, mqfea.data(), mqfea.size());
		fea_cao_.insert(std::make_pair(atoi(black_id.c_str()), fea));
		printf("add new black_id=%s\n", black_id.c_str());
    }
    facedb_locker_.unlock();
}

void FeatureCmpNCao::recvDeleteFeature(const std::string &black_id)
{

    facedb_locker_.lock();
	std::map<int, rr_feature_t>::iterator it = fea_cao_.find(atoi(black_id.c_str()));
    if(it != fea_cao_.end())
    {
        fea_cao_.erase(it);
        printf("delete black_id=%s\n", black_id.c_str());
    }
    else
    {
        printf("This id is not a true id = %s\n", black_id.c_str());
    }
    facedb_locker_.unlock();
}

void FeatureCmpNCao::AddNewFeature(const std::vector<unsigned char> &fea, const std::string &black_id)
{

}

void FeatureCmpNCao::UpDateFeature(const std::vector<unsigned char> &fea, const std::string &black_id)
{

}

