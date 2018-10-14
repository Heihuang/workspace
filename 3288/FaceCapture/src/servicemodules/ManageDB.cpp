#include "ManageDB.h"
#include "../baseutils/RrConfig.h"

namespace rr
{
	ManageDB::ManageDB() :host_name_(""), port_(3306), database_name_(""), user_name_(""), password_("")
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		host_name_ = config.ReadString("MYSQL", "HostName", "127.0.0.1");
		port_ = config.ReadInt("MYSQL", "Port", 3306);
		database_name_ = config.ReadString("MYSQL", "DatabaseName", "localhost");
		user_name_ = config.ReadString("MYSQL", "UserName", "localhost");
		password_ = config.ReadString("MYSQL", "Password", "123456");
	}

	ManageDB::~ManageDB()
	{

	}

	bool ManageDB::OpenDB()
	{
		MySqlDao::getInstance().Init();
		return MySqlDao::getInstance().RealConnect(database_name_.c_str(), host_name_.c_str(), port_, user_name_.c_str(), password_.c_str());
	}

	void ManageDB::CloseDB()
	{
		MySqlDao::getInstance().UnInit();
	}

}

