#include <vector>
#include <unistd.h>
#include "MysqlDao.h"
#include "Managdb.h"
namespace lxh{
Managdb::Managdb() : host_name_(""), port_(0), database_name_(""),
  user_name_(""),  password_("")
{

}

Managdb::~Managdb()
{
}

bool Managdb::OpenDB()
{
    MySqlDao::getInstance().Init();
    return MySqlDao::getInstance().RealConnect(database_name_.c_str(), host_name_.c_str(), port_, user_name_.c_str(), password_.c_str());
}

void Managdb::CloseDB()
{
    MySqlDao::getInstance().UnInit();
}

void Managdb::Query(const std::string& sql)
{
	OpenDB();
	if (!OpenDB())
	{
		printf("open db failed\n");
		return;
	}

	CloseDB();
}

}


