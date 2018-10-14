CC=g++
INCS := -I/FaceOS/thirdparty/glog
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
       src/RrConfig.o				\
       src/BaseUtil.o				\
       src/factory/mqfactory.o   		\
       src/message/messageinfo.o		\
       src/message/recvmessagemq.o		\
       src/message/sendmessageMQ.o		\
       src/servicemodules/DeviceRegist.o	\
       src/servicemodules/DeviceStatusUpdate.o	\
       src/servicemodules/UsbUpdate.o	\
       src/servicemodules/UsbCamaraRegist.o	
Shell:$(OBJS)					
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)	
$(OBJS):%.o:%.cpp				
	$(CC) $(AFLAGS) $^ -o $@ $(LIBS)	
.PHONY:clean Shell					
clean:					
	rm *.o Shell src/*.o
