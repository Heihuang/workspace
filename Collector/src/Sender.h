/*
 * Sender.h
 *
 *  Created on: Jun 16, 2019
 *      Author: zhou
 */

#ifndef SRC_SENDER_H_
#define SRC_SENDER_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "Net.h"
#define Q_MAX_SIZE 128
namespace lxh{
class Sender{
public:
	Sender();
	~Sender();
	int SendMsg(const std::string& msg);
	void Start();
	void Stop();
	static void* work(void* arg);
	inline std::vector<std::string> getMsg()const{
		return msgs_;
	}
	void RealSend();
private:
	int InitNet();
	int UnInitNet();
	Net* net_;
	std::vector<std::string> msgs_;
	bool flg_;
	pthread_t t_id_;
	pthread_mutex_t mtx_;
};

}

#endif /* SRC_SENDER_H_ */
