#include "managdb.h"
#include <vector>
#include <unistd.h>
#include "../baseutils/RrConfig.h"
#include "../baseutils/BaseUtil.h"
#include "../baseutils/MysqlDao.h"

Managdb::Managdb() : activated_("false"), host_name_(""), port_(0), database_name_(""),
  user_name_(""),  password_("")
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

void Managdb::UpdateActivate()
{
	OpenDB();
	if (!OpenDB())
	{
		printf("open db failed\n");
		return;
	}
	std::string sql = "update deviceinfo set DEV_ID = ";
	std::string mac = "'00:00:00:00:00:00'";
	std::vector<std::string> macs;
	rr::GetFirstMacAddress(macs);
	if (!macs.empty())
	{
		mac = "'" +  macs[0] + "'";
	}
	sql += mac;
	sql += " ,is_actived = 'true';";
	bool res = MySqlDao::getInstance().SqlExecute(sql.c_str());
	if (!res)
	{
		printf("exe sql failed! sql=%s", sql.c_str());
	}
	CloseDB();
}

std::string Managdb::QueryIsActivated()
{
	OpenDB();
	if (!OpenDB())
	{
		printf("open db failed\n");
		return activated_;
	}
	std::string sql = "select is_actived from deviceinfo;";
	bool res = MySqlDao::getInstance().SqlExecute(sql.c_str());
	if (!res)
	{
		printf("exe sql failed! sql=%s", sql.c_str());
		CloseDB();
		return activated_;
	}

	while (MySqlDao::getInstance().Next() != NULL)
	{
		if (MySqlDao::getInstance().getRes(0) != NULL)
		{
			activated_.assign(MySqlDao::getInstance().getRes(0));
		}
	}
	MySqlDao::getInstance().FlushDataSet();
	CloseDB();
	return activated_;
}

int Managdb::QueryIsAutoWifi()
{
	OpenDB();
	if (!OpenDB())
	{
		printf("open db failed\n");
		return -1;
	}
	int autowifi = 0;
	std::string sql = "select is_autowifi from deviceinfo;";
	bool res = MySqlDao::getInstance().SqlExecute(sql.c_str());
	if (!res)
	{
		printf("exe sql failed! sql=%s", sql.c_str());
		CloseDB();
		return -1;
	}

	while (MySqlDao::getInstance().Next() != NULL)
	{
		if (MySqlDao::getInstance().getRes(0) != NULL)
		{
			autowifi = atoi(MySqlDao::getInstance().getRes(0));
		}
	}
	MySqlDao::getInstance().FlushDataSet();
	CloseDB();
	return autowifi;
}

void Managdb::UpdateAutoWifi()
{
	OpenDB();
	if (!OpenDB())
	{
		printf("open db failed\n");
		return;
	}
	std::string sql = "update deviceinfo set is_autowifi = 1";
	bool res = MySqlDao::getInstance().SqlExecute(sql.c_str());
	if (!res)
	{
		printf("exe sql failed! sql=%s", sql.c_str());
	}
	CloseDB();
}



