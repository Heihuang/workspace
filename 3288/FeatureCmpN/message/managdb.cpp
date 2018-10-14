#include "managdb.h"
#include <vector>
#include <unistd.h>
#include "../baseutils/RrConfig.h"
#include "MysqlDao.h"

Managdb::Managdb():start_flag_(false),max_black_Id_(0),host_name_(""), port_(0), database_name_(""),
  user_name_(""),  password_(""), feature_cmpn_(NULL)
{
    rr::RrConfig config;
    config.ReadConfig("config.ini");
    host_name_ = config.ReadString("MYSQL", "HostName", "127.0.0.1");
    port_ = config.ReadInt("MYSQL", "Port", 3306);
    database_name_ = config.ReadString("MYSQL", "DatabaseName", "localhost");
    user_name_ = config.ReadString("MYSQL", "UserName", "localhost");
    password_ = config.ReadString("MYSQL", "Password", "123456");
}

Managdb::~Managdb()
{
    QThread::wait();
}

void Managdb::start()
{
    start_flag_ = true;
    pthread_create(&id_, NULL, run, this);
}

void Managdb::stop()
{
    start_flag_ = false;
    pthread_join(id_, NULL);
}

void Managdb::work()
{
    OpenDB();
    getFealist();
    CloseDB();
    while(start_flag_)
    {
       
        OpenDB();
        getScore();
        CloseDB();
        sleep(1);
    }

}

void *Managdb::run(void *param)
{
    Managdb* self = (Managdb*)(param);
    if(self == NULL)
    {
        return;
    }
    self->work();
}

void Managdb::getFealist()
{
    std::string sql = "SELECT comparelist.ID, PHOTO_FEATURE FROM comparelist";
    sql += "inner join comparelistfeature on comparelist.id = comparelistfeature.id where IS_DELETE = 0 and LIST_STATUS =1";
    MySqlDao::getInstance().SqlExecute(sql.c_str());
    std::map<int, std::vector<unsigned  char> > list;
    int id = 0;
    std::vector<unsigned char> fea;
    while (MySqlDao::getInstance().Next() != NULL)
    {
        fea.clear();
        if(MySqlDao::getInstance().getRes(0) != NULL)
        {
            id = atoi(MySqlDao::getInstance().getRes(0));
        }
        if(MySqlDao::getInstance().getRes(1) != NULL)
        {
            fea.assign((unsigned char*)MySqlDao::getInstance().getRes(1));
        }
        list[id] = fea;
    }
    MySqlDao::getInstance().FlushDataSet();
    if(feature_cmpn_)
    {
        feature_cmpn_->recvDBFeature(list);
    }
}

void Managdb::getScore()
{
    std::string sql = "SELECT THRESHOLD_VALUE FROM alarmconfig";
    MySqlDao::getInstance().SqlExecute(sql.c_str());
    float fScore = 1.0;
    while (MySqlDao::getInstance().Next() != NULL)
    {
        if(MySqlDao::getInstance().getRes(0) != NULL)
        {
            fScore = atof(MySqlDao::getInstance().getRes(0));
        }
    }
    MySqlDao::getInstance().FlushDataSet();
    if(feature_cmpn_)
    {
        feature_cmpn_->recvDBScore(fScore);
    }
}

void Managdb::OpenDB()
{
    MySqlDao::getInstance().Init();
    MySqlDao::getInstance().RealConnect(database_name_.c_str(), host_name_.c_str(), port_, user_name_.c_str(), password_.c_str());
}

void Managdb::CloseDB()
{
    MySqlDao::getInstance().UnInit();
}

void Managdb::setFeatureCmp(const FeatureCmp *fea)
{
    feature_cmpn_ = const_cast<FeatureCmp*>(fea);
}


