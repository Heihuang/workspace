/*
 * Net.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: zhou
 */

#include "Net.h"

namespace lxh{
Net::Net():sock_fd_(0){
}

Net::~Net(){

}

int Net::InitConn(const char* ip, const unsigned int port){
	sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&s_addr_, sizeof(s_addr_));
	s_addr_.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &s_addr_.sin_addr);
	s_addr_.sin_port = htons(port);
	return 0;
}

void Net::CloseConn(){
	close(sock_fd_);
}

int Net::Connect(){
	int res =  connect(sock_fd_, (struct sockaddr *)&s_addr_, sizeof(s_addr_));
	if(res == 0)
	{
		return res;
	}
	PrintErrno();
	close(sock_fd_);
}

int Net::Listen(){
	int res = bind(sock_fd_, (struct sockaddr *)&s_addr_, sizeof(s_addr_));
	if (res == 0)
	{
		res = listen(sock_fd_, 128);
	}
	if(res == -1)
	{
		perror("listen error\n");
		close(sock_fd_);
	}
}

size_t Net::Write(char* data, const size_t len){
	if (data == nullptr || len <= 0){
		return 0;
	}
	int wl = write(sock_fd_, data, len);
	if (wl < 0){
		PrintErrno();
		close(sock_fd_);
	}
	return wl;
}

size_t Net::Read(char* buff, size_t len){
	if(buff == nullptr || len <= 0){
		return 0;
	}
	memset((void*)buff, 0, len);
	int rl = read(sock_fd_, buff, len);
	if (rl < 0){
		PrintErrno();
		close(sock_fd_);
	}
	if(rl == 0){

	}
	return rl;
}

void Net::setNonblocking() {
	int opts = fcntl(sock_fd_, F_GETFL);
}

}


