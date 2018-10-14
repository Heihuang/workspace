#ifndef MANAGEDEV_H_
#define MANAGEDEV_H_
#include "MysqlDao.h"
#include "ManageDB.h"
#include <string>
namespace rr
{
	class ManageDev : public ManageDB
	{
	public:
		ManageDev();
		~ManageDev();
		std::string QueryTheDevID();
	private:
		std::string the_dev_id_;
	};
}
#endif
