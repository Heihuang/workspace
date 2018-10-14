CC=g++
INCS := -I/FaceOS/thirdparty/opencv/include/opencv
INCS += -I/FaceOS/thirdparty/opencv/include/opencv2
INCS += -I/FaceOS/thirdparty/opencv/include
INCS += -I/FaceOS/facesdk
INCS += -I/FaceOS/thirdparty/ActiveMQ-CPP/include/activemq-cpp-3.9.4
INCS += -I/FaceOS/thirdparty/apr/include/apr-1
LIBS := -L/FaceOS/thirdparty/opencv/lib				\
	-lopencv_highgui			\
	-lopencv_core				\
	-lopencv_video				\
	-lopencv_calib3d			\
	-lopencv_imgproc
LIBS +=	-L/FaceOS/facesdk/fdc/lib	\
	-lrr_faceverify
LIBS +=	-L/FaceOS/facesdk/fdc/lib	\
	-lfdc
LIBS += -L/FaceOS/thirdparty/ActiveMQ-CPP/lib	\
	-lactivemq-cpp
LIBS += -L/FaceOS/thirdparty/apr/lib	\
	-lapr-1
AFLAGS = -Wall -c -fPIC -O2 -mfpu=neon -std=c++0x $(INCS)
LDFLAGS = -lpthread
LDFLAGS += -Wl,-rpath,/FaceOS/thirdparty/opencv/lib:/FaceOS/facesdk/fdc/lib:/FaceOS/thirdparty/ActiveMQ-CPP/lib:/FaceOS/thirdparty/apr/lib:/FaceOS/facesdk/fdc/lib
OBJS = main.o		 			\
	baseutils/mat2img.o  		\
	baseutils/RrConfig.o		\
	factory/mqfactory.o   		\
	message/messageinfo.o		\
	message/recvmessagemq.o		\
	message/sendmessageMQ.o		\
	servicemodules/basefacesfilter.o		\
	servicemodules/facesfiltercao.o	\
	servicemodules/FactoryAlgorithm.o	\
	servicemodules/FactoryAlgorithmCao.o	
FaceFilter:$(OBJS)					
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)	
$(OBJS):%.o:%.cpp				
	$(CC) $(AFLAGS) $^ -o $@ $(LIBS)	
.PHONY:clean FaceFilter					
clean:					
	rm *.o FaceFilter baseutils/*.o factory/*.o message/*.o servicemodules/*.o
