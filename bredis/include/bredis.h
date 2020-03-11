#ifndef BREDIS_H_
#define BREDIS_H_
#include <vector>
#include <string>
#include "breply.h"
#include "connection.h"

namespace lxh
{
class Bredis
{
private:
    /* data */
public:
    Bredis(/* args */);
    ~Bredis();
    int Init(std::vector<ConnAddr>);
    int Work(Breply &reply, const std::vector<std::string>& args, const std::string& key);
};

Bredis::Bredis(/* args */)
{
}

Bredis::~Bredis()
{
}
}
#endif