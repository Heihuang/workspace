CC = g++ 
INCLUDES := -I./
INCLUDES += -I/home/zhou/local/include
FILTERS =
FILES := $(wildcard *.cpp)
SOURCES = $(filter-out $(FILTERS), $(FILES))
OBJS = $(patsubst %.cpp, %.o, $(SOURCES))

#CXXFLAGS = -O0 -Wall  -g -std=c++11 --no-as-needed 
CXXFLAGS = -O0 -Wall  -g
LDFLAGS := -L/home/zhou/local/lib -static -levent -lc -lrt
LDFLAGS += -Wl,-rpath,/home/zhou/local/lib/

%.o:%.cpp
		$(CC) $(CXXFLAGS) $(INCLUDES) -fPIC -c $< -o $@ 

TARGET=client

all:$(TARGET)
		
$(TARGET): $(OBJS)
		$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
		rm -f $(TARGET) $(OBJS)
