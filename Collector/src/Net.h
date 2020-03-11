/*
 * Net.h
 *
 *  Created on: Jun 26, 2019
 *      Author: zhou
 */

#ifndef SRC_NET_H_
#define SRC_NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>


namespace lxh{
class Net{
public:
	Net();
	~Net();
	int InitConn(const char* ip, const unsigned int port);
	void CloseConn();
	int Connect();
	int Listen();
	size_t Write(char* data, const size_t len);
	size_t Read(char* buff, size_t len);
	void setNonblocking();
	inline void PrintErrno() {
		printf("discover errno = %d\n", errno);
	}
	inline int getSockfd()const{
		return sock_fd_;
	}
private:
	int sock_fd_;
	struct sockaddr_in s_addr_;
};

}

#endif /* SRC_NET_H_ */
