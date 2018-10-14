#ifndef FACTORYALGORITHMFDC_H_
#define FACTORYALGORITHMFDC_H_
#include "FactoryAlgorithm.h"

namespace rr
{
class FactoryAlgorithmfdc :public FactoryAlgorithm
{
public:
	FactoryAlgorithmfdc();
	virtual ~FactoryAlgorithmfdc();
	virtual void CreateFaceDetector(FaceDetector** object);
	virtual void DestroyFaceDetector(FaceDetector* object);
};
}
#endif