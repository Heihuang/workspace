/*
 * Net.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: zhou
 */

#include "Net.h"

namespace lxh {
Net::Net() :
		sock_fd_(0) {
}

Net::~Net() {

}

int Net::InitConn(const char* ip, const unsigned int port) {
	sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&s_addr_, sizeof(s_addr_));
	s_addr_.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &s_addr_.sin_addr);
	s_addr_.sin_port = htons(port);
	return 0;
}

void Net::CloseConn() {
	close(sock_fd_);
}

int Net::Connect() {
	int res = connect(sock_fd_, (struct sockaddr *) &s_addr_, sizeof(s_addr_));
	if (res == 0) {
		return res;
	}
	PrintErrno();
	close(sock_fd_);
	return res;
}

int Net::Listen() {
	int res = bind(sock_fd_, (struct sockaddr *) &s_addr_, sizeof(s_addr_));
	if (res == 0) {
		res = listen(sock_fd_, 128);
	}
	if (res == -1) {
		perror("listen error\n");
		close(sock_fd_);
	}
	return res;
}

size_t Net::Write(const int& fd, char* data, const size_t len) {
	if (data == nullptr || len <= 0) {
		return 0;
	}
	int wl = write(fd, data, len);
	if (wl < 0) {
		PrintErrno();
		close(fd);
	}
	return wl;
}

size_t Net::Read(const int& fd, char* buff, size_t len) {
	if (buff == nullptr || len <= 0) {
		return 0;
	}
	memset((void*) buff, 0, len);
	int rl = read(fd, buff, len);
	if (rl < 0) {
		PrintErrno();
		close(fd);
	}
	else if (rl == 0) {

	}
	else{
		printf("%s\n", buff);
	}
	return rl;
}

int Net::Accept() {
	socklen_t clilen;
	struct sockaddr_in clientaddr;
	int connfd = accept(sock_fd_, (sockaddr*) &clientaddr, &clilen);
	if (connfd < 0) {
		perror("connfd<0");
		exit(1);
	}
	char *c_ip = inet_ntoa(clientaddr.sin_addr);
	printf("accapt a connection from %s\n", c_ip);
	return connfd;
}

void Net::setNonblocking() {
	int opts = fcntl(sock_fd_, F_GETFL);
	if (opts < 0){
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts|O_NONBLOCK;
	if(sock_fd_, F_SETFL, opts){
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}

}

