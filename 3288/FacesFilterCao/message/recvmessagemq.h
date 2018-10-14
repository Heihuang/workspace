#ifndef RECVMESSAGEMQ_H
#define RECVMESSAGEMQ_H
#include <map>
#include "../factory/mqfactory.h"
#include "../servicemodules/basefacesfilter.h"
#include "../servicemodules/FactoryAlgorithm.h"
#include "cv.h"
#include "cxcore.hpp"
#include "opencv.hpp"
#include "highgui.h"
#include "../baseutils/mat2img.h"

class RecvmessageMQ
{

public:
    RecvmessageMQ();
    ~RecvmessageMQ();
    void CreateConsumer();
    void RecvMsg(const std::string& taskid, const std::vector<unsigned char>& img);
private:
    Consumer* consumer_;
    BaseFacesFilter *face_filter_;
    FactoryAlgorithm* algorithm_;
};
#endif // RECVMESSAGEMQ_H
