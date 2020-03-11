/*
 * Epoll.cpp
 *
 *  Created on: Jun 28, 2019
 *      Author: zhou
 */
#include "Epoll.h"

namespace lxh {

Epoll::Epoll() :
		epollop_(nullptr), net_(nullptr),hcp_(nullptr),user_(nullptr),p_flag_(false) {

}

Epoll::~Epoll() {

}

void Epoll::EpollInit() {
	int epfd = -1;
	if (epfd == -1) {
		if ((epfd = epoll_create(INITIAL_NEVENT)) == -1) {
			if (errno != ENOSYS)
				printf("epoll_create failed\n");
			return;
		}
	}
	if (!(epollop_ = (struct epollop *)calloc(1, sizeof(struct epollop)))) {
		close(epfd);
		return;
	}
	epollop_->epfd = epfd;
	epollop_->events = (struct epoll_event *)calloc(INITIAL_NEVENT, sizeof(struct epoll_event));
	if (epollop_->events == nullptr) {
		free(epollop_);
		close(epfd);
		return;
	}
	epollop_->nevents = INITIAL_NEVENT;
	//bind listen_fd_
	if (net_) {
		chargeepoll(net_->getSockfd(), EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
	}
	p_flag_ = true;
}

void Epoll::EpollDispatch() {
	if (epollop_ == nullptr) {
		return;
	}
	if(hcp_ == nullptr || user_ == nullptr){
		return;
	}
	char buff[MAXLEN] = {0};
	size_t sz = 0;
	int sk_fd = -1;
	do{
		int nfds = epoll_wait(epollop_->epfd, epollop_->events,epollop_->nevents, EPOLL_TIMEOUT);
		for (int i = 0; i < nfds; ++i) {
			//listen fd
			if (epollop_->events[i].data.fd == net_->getSockfd()) {
				int cfd = net_->Accept();
				chargeepoll(cfd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
			} else if (epollop_->events[i].events & EPOLLIN) {
				if ((sk_fd = epollop_->events[i].data.fd) < 0) {
					continue;
				}
				if ((sz = net_->Read(sk_fd, buff, MAXLEN)) < 0) {
					printf("DEBUG %s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__);
					sk_fd = -1;
				}
				else if(sz > 0){
					hcp_(buff, sz, user_);
				}
				chargeepoll(sk_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
			} else if (epollop_->events[i].events & EPOLLOUT) {
				if ((sk_fd = epollop_->events[i].data.fd) < 0) {
					continue;
				}
				/*if ((sz = net_->Write(sk_fd, buff, MAXLEN)) < 0) {
					sk_fd = -1;
				}*/
				chargeepoll(sk_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);
			}
		}
	}while (p_flag_);
}

void Epoll::EpollClose(){
	p_flag_ = false;
	if(epollop_){
		if(epollop_->events){
			free(epollop_->events);
			epollop_->events = nullptr;
			close(epollop_->epfd);
		}
		free(epollop_);
		epollop_ = nullptr;
	}
}
void Epoll::chargeepoll(int fd, __uint32_t evt, __uint32_t opt) {
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = evt;
	epoll_ctl(epollop_->epfd, opt, fd, &ev);
}
}

