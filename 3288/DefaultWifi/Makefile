CC=g++
AFLAGS = -Wall -c -fPIC -O2 -std=c++11
LDFLAGS = -lpthread -lmysqlclient
OBJS = main.o		 		\
	baseutils/RrConfig.o		\
	baseutils/BaseUtil.o		\
	servicemodules/managdb.o	
DefaultWifi:$(OBJS)					
	$(CC) -o $@ $^ $(LDFLAGS)
$(OBJS):%.o:%.cpp				
	$(CC) $(AFLAGS) $^ -o $@	
.PHONY:clean DefaultWifi					
clean:					
	rm *.o DefaultWifi baseutils/*.o servicemodules/*.o
