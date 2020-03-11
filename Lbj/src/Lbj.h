/*
 * Lbj.h
 *
 *  Created on: Jul 10, 2019
 *      Author: zhou
 */

#ifndef SRC_LBJ_H_
#define SRC_LBJ_H_
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>

#include<event.h>
#include<event2/bufferevent.h>
#include<event2/buffer.h>
#include<event2/util.h>

namespace lxh {

class Lbj {
public:
	explicit Lbj(std::string ip, int port);
	virtual ~Lbj();
	void WakeUp();
private:
	int ConnectServer();
	static void MsgTimer(int sock, short event, void *arg);
	static void MsgReadCb(struct bufferevent* bev, void* arg);
	static void EventCb(struct bufferevent *bev, short event, void *arg);
private:
	struct sockaddr_in server_addr_;
	int port_;
	std::string ip_;
	struct event_base* base_;
	struct bufferevent* bev_;
};

} /* namespace lxh */

#endif /* SRC_LBJ_H_ */
