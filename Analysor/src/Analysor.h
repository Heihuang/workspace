/*
 * Analysor.h
 *
 *  Created on: Jun 28, 2019
 *      Author: zhou
 */

#ifndef SRC_ANALYSOR_H_
#define SRC_ANALYSOR_H_
#include <vector>
#include <string>
#include "Net.h"
#include "Epoll.h"

namespace lxh {
class Analysor {
public:
	Analysor();
	~Analysor();
	void Start();
	void Close();
	static void* HandleSaveMsg(void* args);
	static void HandleCp(void* data, size_t len, void* user);
	void CpData(void* data, size_t len);
	void SaveMsg();
private:
	Net *net_;
	Epoll *ep_;
	std::vector<std::string> msgs_;
private:
	void listennet();
	void closenet();
};
}

#endif /* SRC_ANALYSOR_H_ */
