#ifndef LOGPROCESSOR
#define LOGPROCESSOR

#include <stdio.h>

#include "glog/logging.h"
#include "glog/raw_logging.h"

#define OutputLog_Info(format, ...) \
{\
	char szLog[1024 * 5] = { 0 }; \
	sprintf(szLog, format, ##__VA_ARGS__); \
	LOG(INFO) << szLog; \
}

#define OutputLog_Warning(format, ...) \
{\
	char szLog[1024 * 5] = { 0 }; \
	sprintf(szLog, format, ##__VA_ARGS__); \
	LOG(WARNING) << szLog; \
}

#define OutputLog_Error(format, ...) \
{\
	char szLog[1024 * 5] = { 0 }; \
	sprintf(szLog, format, ##__VA_ARGS__); \
	LOG(ERROR) << szLog; \
}

//将信息输出到单独的文件和 LOG(ERROR)
void SignalHandle(const char* data, int size);

class LogProcessor
{
public:
	//GLOG配置：
	LogProcessor();
	//GLOG内存清理：
	~LogProcessor() { google::ShutdownGoogleLogging(); }
};

#endif // LOGPROCESSOR

