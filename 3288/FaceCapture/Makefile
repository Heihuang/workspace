CC=g++
INCS := -I/FaceOS/thirdparty/opencv/include/opencv
INCS += -I/FaceOS/thirdparty/opencv/include/opencv2
INCS += -I/FaceOS/thirdparty/opencv/include
INCS += -I/FaceOS/facesdk
INCS += -I/FaceOS/thirdparty/ActiveMQ-CPP/include/activemq-cpp-3.9.4
INCS += -I/FaceOS/thirdparty/apr/include/apr-1
INCS += -I/FaceOS/thirdparty/glog/include
LIBS := -L/FaceOS/thirdparty/opencv/lib				\
	-lopencv_highgui			\
	-lopencv_core				\
	-lopencv_video				\
	-lopencv_calib3d			\
	-lopencv_imgproc
LIBS += -L/FaceOS/facesdk/fdc/lib	\
	-lfdc			
LIBS += -L/FaceOS/thirdparty/ActiveMQ-CPP/lib	\
	-lactivemq-cpp
LIBS += -L/FaceOS/thirdparty/apr/lib	\
	-lapr-1
LIBS += -L/FaceOS/thirdparty/glog/lib	\
	-lglog
AFLAGS = -Wall -c -g -fPIC -O2 $(INCS)
LDFLAGS = -lpthread -lmysqlclient
LDFLAGS += -Wl,-rpath,/FaceOS/thirdparty/opencv/lib:/FaceOS/facesdk/fdc/lib:/FaceOS/thirdparty/ActiveMQ-CPP/lib:/FaceOS/thirdparty/apr/lib
OBJS = main.o		 			\
	src/baseutils/Mat2Img.o  		\
	src/baseutils/RrConfig.o		\
	src/baseutils/BaseUtil.o		\
	src/factory/Mqfactory.o   		\
	src/message/Messageinfo.o		\
	src/message/Recvmessagemq.o		\
	src/message/SendmessageMQ.o		\
	src/servicemodules/Camera.o		\
	src/servicemodules/FacesDetector.o	\
	src/servicemodules/FacesDetectorFdc.o	\
	src/servicemodules/FactoryAlgorithm.o	\
	src/servicemodules/FactoryAlgorithmfdc.o	\
	src/servicemodules/v4l2uvc.o		\
	src/servicemodules/SendVideo.o	\
	src/servicemodules/IPCamera.o	\
	src/servicemodules/ManageCamera.o	\
	src/servicemodules/SftVideo.o	\
	src/servicemodules/UsbCamera.o	\
	src/servicemodules/Video.o	\
	src/servicemodules/ManageDB.o	\
	src/servicemodules/ManageDev.o	\
	logprocessor.o
FaceCapture:$(OBJS)					
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)	
$(OBJS):%.o:%.cpp				
	$(CC) $(AFLAGS) $^ -o $@ $(LIBS)	
.PHONY:clean FaceCapture					
clean:					
	rm *.o FaceCapture src/baseutils/*.o src/factory/*.o src/message/*.o src/servicemodules/*.o
