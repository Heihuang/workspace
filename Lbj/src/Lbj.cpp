/*
 * Lbj.cpp
 *
 *  Created on: Jul 10, 2019
 *      Author: zhou
 */

#include "Lbj.h"

namespace lxh {

Lbj::Lbj(std::string ip, int port) :
	ip_(ip), port_(port), base_(nullptr), bev_(nullptr) {
	// TODO Auto-generated constructor stub
	memset(&server_addr_, 0, sizeof(server_addr_));
	server_addr_.sin_family = AF_INET;
	server_addr_.sin_port = htons(port_);
	inet_aton(ip_.c_str(), &server_addr_.sin_addr);

}

Lbj::~Lbj() {
	// TODO Auto-generated destructor stub
}

void Lbj::WakeUp() {
	int sockfd = ConnectServer();
	base_ = event_base_new();
	bev_ = bufferevent_socket_new(base_, sockfd, BEV_OPT_CLOSE_ON_FREE);
	struct timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	struct event* ev_cmd = event_new(base_, sockfd, EV_TIMEOUT | EV_PERSIST, MsgTimer, (void*)bev_);
	event_add(ev_cmd, &tv);
	//当socket关闭时会用到回调参数
	bufferevent_setcb(bev_, MsgReadCb, nullptr, EventCb, (void*) ev_cmd);
	bufferevent_enable(bev_, EV_READ | EV_PERSIST);
	event_base_dispatch(base_);
}

typedef struct sockaddr SA;
int Lbj::ConnectServer() {
	int sockfd = ::socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		return sockfd;
	}
	int res = ::connect(sockfd, (const SA*) &server_addr_, sizeof(server_addr_));
	if (res == -1) {
		::close(sockfd);
	}
	evutil_make_socket_nonblocking(sockfd);
	return sockfd;
}

void Lbj::MsgTimer(int sock, short event, void *arg) {
	const char* msg = "12";
	printf("msg=%s\n", msg);
	struct bufferevent* bev = (struct bufferevent*) arg;
	//把消息发送给服务器端
	bufferevent_write(bev, msg, strlen(msg));
}

void Lbj::MsgReadCb(struct bufferevent* bev, void* arg) {
	char msg[1024];

	size_t len = bufferevent_read(bev, msg, sizeof(msg));
	msg[len] = '\0';

	printf("recv %s from server\n", msg);
}

void Lbj::EventCb(struct bufferevent *bev, short event, void *arg) {
	if (event & BEV_EVENT_EOF)
		printf("connection closed\n");
	else if (event & BEV_EVENT_ERROR)
		printf("some other error\n");

	//这将自动close套接字和free读写缓冲区
	bufferevent_free (bev);

	struct event *ev = (struct event*) arg;
	//因为socket已经没有，所以这个event也没有存在的必要了
	event_free(ev);
}
} /* namespace lxh */
