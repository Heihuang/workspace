#include "ManageDev.h"

namespace rr
{

	ManageDev::ManageDev() :the_dev_id_("")
	{

	}

	ManageDev::~ManageDev()
	{

	}

	std::string ManageDev::QueryTheDevID()
	{
		if (!OpenDB())
		{
			printf("open db failed\n");
			return the_dev_id_;
		}
		std::string sql = "select item_value from of_sys_system_info where item_name = 'DevID';";
		MySqlDao::getInstance().SqlExecute(sql.c_str());
		while (MySqlDao::getInstance().Next() != NULL)
		{
			if (MySqlDao::getInstance().getRes(0) != NULL)
			{
				the_dev_id_.assign(MySqlDao::getInstance().getRes(0));
			}
		}
		MySqlDao::getInstance().FlushDataSet();
		CloseDB();
		return the_dev_id_;
	}

}

