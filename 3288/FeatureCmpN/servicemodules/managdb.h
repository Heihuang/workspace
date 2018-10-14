#ifndef MANAGDB_H
#define MANAGDB_H
#include <vector>
#include <map>
#include <string>
#include <pthread.h>
#include "featurecmp.h"
#include "featurecmpncao.h"

class Managdb
{
public:
     Managdb();
     ~Managdb();
     void start();
     void stop();
     void setFeatureCmp(const FeatureCmp* fea);
private:
     bool start_flag_;
     std::string host_name_;
     int port_;
     std::string database_name_;
     std::string user_name_;
     std::string password_;

     static void* run(void* param);
     void work();
     void getFealist();
     void getScore();
     bool OpenDB();
     void CloseDB();
private:
    int max_black_Id_;
    int last_Id_;
    FeatureCmp *feature_cmpn_;
    pthread_t id_;
};
#endif // MANAGDB_H
