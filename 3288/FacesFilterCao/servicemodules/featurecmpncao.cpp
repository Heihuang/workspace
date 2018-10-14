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
    std::map<int, float*>::iterator it = fea_cao_.begin();
    while(it != fea_cao_.end())
    {
        float* fea = it->second;
        delete[] fea;
        fea = NULL;
        it++;
    }
    fea_cao_.clear();
    facedb_locker_.unlock();
}

void FeatureCmpNCao::recvMQFeature(const std::vector<unsigned char> &mqfea, const std::string &taskid)
{
    printf("sert\n");
    if(!srcfea_flag_)
    {
        return;
    }
    float* fea = new float[FEATURES_LEN];
    unsigned char data[FEATURES_LEN * 4];
    printf("rt fea size=%d\n", mqfea.size());
    if(mqfea.size() == FEATURES_LEN * 4)
    {
        memcpy(data, mqfea.data(), mqfea.size());
        for(int i = 0; i < FEATURES_LEN; i++)
        {
            float f;
            memcpy(&f, data + i*4, 4);
            fea[i] = f;
        }
        serchFace(fea, taskid);
    }
    delete[] fea;
}

void FeatureCmpNCao::recvDBFeature(const std::map<int, std::vector<unsigned char> > &dbfea)
{
    if(dbfea.empty())
    {
        return;
    }
    unsigned char data[FEATURES_LEN * 4];
    std::map<int, std::vector<unsigned char> >::const_iterator it = dbfea.begin();
    while(it != dbfea.end())
    {
        std::vector<unsigned char> tmp = it->second;
        if(tmp.size() == FEATURES_LEN * 4)
        {
            float* fea = new float[FEATURES_LEN];
            memcpy(data, tmp.data(), tmp.size());
            for(int i = 0; i < FEATURES_LEN; i++)
            {
                float f;
                memcpy(&f, data + i*4, 4);
                fea[i] = f;
            }
            fea_cao_.insert(std::make_pair(it->first, fea));
            printf("Add blackfea, blackid=%d\n", it->first);
        }
        it++;
    }
    srcfea_flag_ = true;
}

void FeatureCmpNCao::recvDBScore(const float &score)
{
    score_ = score;
    printf("threshold = %f\n", score);
}

void FeatureCmpNCao::serchFace(float *fea, const std::string &task_id)
{
    std::map<float, int> serch_map;
    std::map<float, int> min_map;
    float sc = 0.0;
    int Id = 0;
    std::map<int, float*>::iterator it = fea_cao_.begin();
    while(it != fea_cao_.end())
    {
        float* fea_src = it->second;
        sc = Calcsimilarity(fea_src, fea);
        if(sc > 1)
        {
            for(int i = 0; i < 512; ++i)
            {
                printf("f1=[%f,]",fea[i]);
            }
            for(int i = 0; i < 512; ++i)
            {
                printf("f2=[%f,]",fea_src[i]);
            }
        }
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


float FeatureCmpNCao::Calcsimilarity(float *feat_src, float *feat_dst)
{
    float similarity = 0.0;
    float d= 0.0;
    float n1 = 0.0;
    float n2 = 0.0;
    for(int k = 0; k < FEATURES_LEN; k++)
    {
        d += feat_src[k] * feat_dst[k];
        n1 += feat_src[k] * feat_dst[k];
        n2 += feat_dst[k] * feat_dst[k];
    }
    if(n1==0 || n2==0)
    {
        printf("fea n1 =%d fea n2 =%d \n", n1, n2);
    }
    else
    {
        similarity = d / sqrt(n1) / sqrt(n2);
    }
    //cv::Mat m1(1, 512, CV_32FC1, feat_src);
    //cv::Mat m2(1, 512, CV_32FC1, feat_dst);
    //float similarity = m1.dot(m2) /( (sqrt(m1.dot(m1))) * sqrt(m2.dot(m2)) );
    return similarity;
}

void FeatureCmpNCao::recvUpdateFeature(const std::vector<unsigned char> &mqfea, const std::string &black_id)
{
    unsigned char data[FEATURES_LEN * 4];
    facedb_locker_.lock();
    std::map<int, float*>::iterator it = fea_cao_.find(atoi(black_id.c_str()));
    if(it != fea_cao_.end())
    {
        fea_cao_.erase(it);
        if(mqfea.size() == FEATURES_LEN * 4)
        {
            float* fea = new float[FEATURES_LEN];
            memcpy(data, mqfea.data(), mqfea.size());
            for(int i = 0; i < FEATURES_LEN; i++)
            {
                float f;
                memcpy(&f, data + i*4, 4);
                fea[i] = f;
            }
            fea_cao_.insert(std::make_pair(atoi(black_id.c_str()), fea));
            printf("update black_id=%s\n", black_id.c_str());
        }
    }
    else
    {
        if(mqfea.size() == FEATURES_LEN * 4)
        {
            float* fea = new float[FEATURES_LEN];
            memcpy(data, mqfea.data(), mqfea.size());
            for(int i = 0; i < FEATURES_LEN; i++)
            {
                float f;
                memcpy(&f, data + i*4, 4);
                fea[i] = f;
            }
            fea_cao_.insert(std::make_pair(atoi(black_id.c_str()), fea));
            printf("add new black_id=%s\n", black_id.c_str());
        }
    }
    facedb_locker_.unlock();
    srcfea_flag_ = true;
}

void FeatureCmpNCao::recvDeleteFeature(const std::string &black_id)
{

    facedb_locker_.lock();
    std::map<int, float*>::iterator it = fea_cao_.find(atoi(black_id.c_str()));
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

