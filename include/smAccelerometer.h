#pragma once
#include <s3eLocation.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmAccelerometer: public CsmFeature
	{
	public: 
		virtual ~CsmAccelerometer();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		static bool IsAvailable();
		static int GetX();
		static int GetY();
		static int GetZ();
	protected:
		static CsmAccelerometer* RequestFeature();
		virtual void StartFeature();
		virtual void StopFeature();
	};
}