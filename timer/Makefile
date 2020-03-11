CC = g++
INCLUDES = -I./
FILTERS =
FILES := $(wildcard *.cpp)
SOURCES = $(filter-out $(FILTERS), $(FILES))
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

CXXFLAGS = -O0 -Wall -std=c++11  -g 
LDFLAGS = 
%.o:%.cpp
	$(CC) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -fPIC -c $< -o $@

TARGET=Timer

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -f $(TARGET) $(OBJS)