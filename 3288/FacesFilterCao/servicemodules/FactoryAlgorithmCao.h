#ifndef FACTORYALGORITHMCAO_H_
#define FACTORYALGORITHMCAO_H_
#include "FactoryAlgorithm.h"

class FactoryAlgorithmCao :
	public FactoryAlgorithm
{
public:
	FactoryAlgorithmCao();
	virtual ~FactoryAlgorithmCao();
    virtual void CreateFaceVerify(BaseFacesFilter** object);
    virtual void DestroyFaceVerify(BaseFacesFilter* object);
};
#endif
