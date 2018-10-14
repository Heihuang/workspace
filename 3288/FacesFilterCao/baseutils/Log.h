#include <stdio.h>
#include <stdlib.h>
#define LOG(fmt,...) printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__) 