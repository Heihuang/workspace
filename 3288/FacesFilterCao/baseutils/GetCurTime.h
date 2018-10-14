#ifndef GET_CURTIME_H_
#define GET_CURTIME_H_
#include <string>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
namespace rr
{
static std::string getCurTime()
{
	std::string ct;
	struct timeval tv;
	char date[64];
	char buf[256];
	gettimeofday(&tv, NULL);
	strftime(date, sizeof(buf)-1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
	sprintf(buf, "%s.%03d", date, (int)(tv.tv_usec / 1000));
	ct.assign(buf, strlen(buf));
	return ct;
}

static int getSecs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
}

static unsigned long getTickCount()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	return (ts.tv_sec*1000 + ts.tv_nsec/1000000);
}
}
#endif
