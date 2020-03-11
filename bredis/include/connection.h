#ifndef CONNECTION_H_
#define CONNECTION_H_
#include <string>
#include <vector>

namespace lxh
{
    
typedef struct ConnAddr_
{
    /* data */
    ConnAddr_(const std::string& ip, const unsigned int port)
    {
        ip_ = ip;
        port_ = port;
    }
    std::string ip_;
    unsigned int port_;
}ConnAddr;

class Connection
{
public:
    /* data */
    Connection();
    ~Connection();
};


}


#endif // !CONNECTION_H_
