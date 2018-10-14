#include "FactoryAlgorithmCao.h"
#include "facesfiltercao.h"

FactoryAlgorithmCao::FactoryAlgorithmCao()
{
}


FactoryAlgorithmCao::~FactoryAlgorithmCao()
{
}

void FactoryAlgorithmCao::CreateFaceVerify(BaseFacesFilter** object)
{
	try
	{
        *object = new FacesFilterCao();
	}
	catch (...)
	{
	}
}

void FactoryAlgorithmCao::DestroyFaceVerify(BaseFacesFilter* object)
{
	if (object != NULL)
	{
		delete object;
		object = NULL;
	}
}
