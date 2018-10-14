#include "FactoryAlgorithmfdc.h"
#include "FacesDetectorFdc.h"

namespace rr
{

	FactoryAlgorithmfdc::FactoryAlgorithmfdc()
	{

	}

	FactoryAlgorithmfdc::~FactoryAlgorithmfdc()
	{

	}

	void FactoryAlgorithmfdc::CreateFaceDetector(FaceDetector** object)
	{
		try
		{
			*object = new FaceDetectorFdc();
		}
		catch (...)
		{
		}
	}

	void FactoryAlgorithmfdc::DestroyFaceDetector(FaceDetector* object)
	{
		if (object)		{			delete object;			object = NULL;		}
	}

}

