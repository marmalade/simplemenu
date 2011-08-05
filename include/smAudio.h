#pragma once
#include <s3eLocation.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmAudio: public CsmFeature
	{
	public: 
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();

		static bool IsAvailable();
		static bool PlayFile(const char*, int repeatCount);
	};
}