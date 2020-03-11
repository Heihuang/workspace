CC = g++ 
INCLUDES = -I./
FILTERS =
FILES := $(wildcard *.cc)
SOURCES = $(filter-out $(FILTERS), $(FILES))
OBJS = $(patsubst %.cc, %.o, $(SOURCES))

CXXFLAGS = -O0 -Wall -std=c++11  -g 
LDFLAGS = 
#LDFLAGS = -L../lib64 -liparea

%.o:%.cc
		$(CC) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -fPIC -c $< -o $@

TARGET=server

all:$(TARGET)
		
$(TARGET): $(OBJS)
		$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
		rm -f $(TARGET) $(OBJS)
