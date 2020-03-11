CC = g++ 
INCLUDES := -I./
INCLUDES += -I/home/zhou/local/include
FILTERS =
FILES := $(wildcard *.cpp ./src/*.cpp)
SOURCES = $(filter-out $(FILTERS), $(FILES))
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

CXXFLAGS = -O0 -Wall  -g -std=c++11

%.o:%.cpp
		$(CC) $(CXXFLAGS) $(INCLUDES) -fPIC -c $< -o $@ 

TARGET=test

all:$(TARGET)
		
$(TARGET): $(OBJS)
		$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
		rm -f $(TARGET) $(OBJS)
