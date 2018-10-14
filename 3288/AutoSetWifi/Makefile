CC=g++
INCS += -I/FaceOS/thirdparty/ActiveMQ-CPP/include/activemq-cpp-3.9.4
INCS += -I/FaceOS/thirdparty/apr/include/apr-1
LIBS := -L/FaceOS/thirdparty/ActiveMQ-CPP/lib	\
	-lactivemq-cpp
LIBS += -L/FaceOS/thirdparty/apr/lib	\
	-lapr-1
AFLAGS = -Wall -c -g -fPIC -O2 $(INCS)
LDFLAGS = -lpthread
LDFLAGS += -Wl,-rpath,/FaceOS/thirdparty/ActiveMQ-CPP/lib:/FaceOS/thirdparty/apr/lib
OBJS = main.o		 			\
       src/baseutils/RrConfig.o			\
       src/baseutils/BaseUtil.o			\
       src/factory/mqfactory.o   		\
       src/message/recvmessagemq.o		\
       src/servicemodules/AutoSetWifi.o	
AutoSetWifi:$(OBJS)					
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)	
$(OBJS):%.o:%.cpp				
	$(CC) $(AFLAGS) $^ -o $@ $(LIBS)	
.PHONY:clean AutoSetWifi					
clean:					
	rm *.o AutoSetWifi src/baseutils/*.o src/factory/*.o src/message/*.o src/servicemodules/*.o
