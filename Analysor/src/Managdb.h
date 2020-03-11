#ifndef MANAGDB_H
#define MANAGDB_H
#include <vector>
#include <map>
#include <string>
namespace lxh{
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
     bool OpenDB();
     void CloseDB();
public:
	void Query(const std::string& sql);
};
}
#endif // MANAGDB_H
