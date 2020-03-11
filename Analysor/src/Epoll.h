/*
 * Epoll.h
 *
 *  Created on: Jun 28, 2019
 *      Author: zhou
 */

#ifndef SRC_EPOLL_H_
#define SRC_EPOLL_H_

#include <stdint.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/queue.h>
#include <sys/epoll.h>
#include <signal.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "Net.h"
#define INITIAL_NEVENT 32
#define MAX_NEVENT 4096
#define EPOLL_TIMEOUT 200
#define MAXLEN 1024

namespace lxh{

struct epollop {
	struct epoll_event *events;
	int nevents;
	int epfd;
};
typedef void(*HandleCp)(void* data, size_t len, void* user);

class Epoll{
public:
	Epoll();
	~Epoll();
	void EpollInit();
	void EpollDispatch();
	void EpollClose();
	void setNet(const Net* net){
		net_ = const_cast<Net*>(net);
	}
	void setHcp(HandleCp hcp){
		hcp_ = hcp;
	}
private:
	struct epollop *epollop_;
	Net *net_;
	HandleCp hcp_;
	void* user_;
	bool p_flag_;
private:
	void chargeepoll(int fd, __uint32_t evt, __uint32_t opt);
};
}



#endif /* SRC_EPOLL_H_ */
