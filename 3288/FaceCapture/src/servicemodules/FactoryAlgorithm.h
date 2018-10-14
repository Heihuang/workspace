#ifndef FACTORYALGORITHM_H_
#define FACTORYALGORITHM_H_
#include "FacesDetector.h"

namespace rr
{
class FactoryAlgorithm
{
public:
	FactoryAlgorithm();
	virtual ~FactoryAlgorithm();
	virtual void CreateFaceDetector(FaceDetector** object) = 0;
	virtual void DestroyFaceDetector(FaceDetector* object) = 0;
};
}

#endif
