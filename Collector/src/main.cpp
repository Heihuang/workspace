/*
 * main.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: zhou
 */
#include <thread>
#include <chrono>
#include "Collector.h"
#include "CltRedisInfo.h"
#include "timer/timer.h"

static std::shared_ptr<lxh::Collector> gct =
		std::make_shared<lxh::CltRedisInfo>();
void TimerHandler() {
	gct->Collect();
}

int main(int argc, char** argv) {
	gct->Ready();
	TimerManager tm;
	Timer t(tm);
	t.Start(&TimerHandler, 1000);
	while (true) {
		tm.DetectTimers();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	gct->Down();
	return 0;
}

