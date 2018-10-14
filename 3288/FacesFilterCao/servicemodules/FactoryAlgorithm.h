#ifndef FACTORYALGORITHM_H_
#define FACTORYALGORITHM_H_
#include "basefacesfilter.h"

class FactoryAlgorithm
{
public:
	FactoryAlgorithm();
	virtual ~FactoryAlgorithm();
    virtual void CreateFaceVerify(BaseFacesFilter** object) = 0;
    virtual void DestroyFaceVerify(BaseFacesFilter* object) = 0;
};
#endif
