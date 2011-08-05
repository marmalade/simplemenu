#pragma once
#include <s3eCamera.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmCamera: public CsmFeature
	{
	public: 
		virtual ~CsmCamera();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
	protected:
		static CsmCamera* RequestFeature();
		//static int32 Callback (void* systemData, void* userData);
		virtual void StartFeature();
		virtual void StopFeature();
	};
}