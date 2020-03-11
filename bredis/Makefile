CXX = /opt/rh/devtoolset-3/root/usr/bin/g++ 
INCLUDES = -I./
INCLUDES += -I/home/homework/include
FILTERS =
FILES := $(wildcard src/*.cpp)
SOURCES = $(filter-out $(FILTERS), $(FILES))
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

CXXFLAGS = -O0 -Wall -std=c++11  -g 
LDFLAGS =  -lpthread  
SOFLAGS = -shared
#LDFLAGS = -L../lib64 -liparea

%.o:%.cpp
		$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(SOFLAGS) -fPIC -c $< -o $@

TARGET=libbredis

all:$(TARGET)
		
$(TARGET): $(OBJS)
		$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
		rm -f $(TARGET) $(OBJS)
