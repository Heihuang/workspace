#include <stdio.h>
#include <string>
#include "src/Lbj.h"

int main(int argc, char **argv) {
	std::string ip("127.0.0.1");
	int port = 9999;
	lxh::Lbj b(ip, port);
	b.WakeUp();
	//b.Call();
	return 0;
}
