#pragma once
#include <s3eLocation.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmCompass: public CsmFeature
	{
	public: 
		~CsmCompass();
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();

		static bool IsAvailable();

		static int GetX();
		static int GetY();
		static int GetZ();
	protected:
		static CsmCompass* RequestFeature();
		virtual void StartFeature();
		virtual void StopFeature();
	};
}