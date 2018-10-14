#ifndef MANAGEDB_H_
#define MANAGEDB_H_
#include "MysqlDao.h"
#include <string>
namespace rr
{
	class ManageDB
	{
	public:
		ManageDB();
		virtual ~ManageDB();

	protected:
		bool OpenDB();
		void CloseDB();
		std::string host_name_;
		int port_;
		std::string database_name_;
		std::string user_name_;
		std::string password_;
	};
}

#endif
