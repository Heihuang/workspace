#ifndef MANAGDB_H
#define MANAGDB_H
#include <vector>
#include <map>
#include <string>

class Managdb
{
public:
     Managdb();
     ~Managdb();
private:
     std::string host_name_;
     int port_;
     std::string database_name_;
     std::string user_name_;
     std::string password_;
	 std::string activated_;
     bool OpenDB();
     void CloseDB();
public:
	std::string QueryIsActivated();
	void UpdateActivate();
	int QueryIsAutoWifi();
	void UpdateAutoWifi();
};
#endif // MANAGDB_H
