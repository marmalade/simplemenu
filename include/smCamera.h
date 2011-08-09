#pragma once
#include <s3eCamera.h>
#include <smImageSource.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmCamera: public CsmFeature
	{
	protected:
		bool feedUpdated;
		bool started;
	public: 
		CsmCamera();
		virtual ~CsmCamera();
		//Is feature available on device
		static bool IsAvailable();
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
	protected:
		static CsmCamera* RequestFeature();
		//static int32 Callback (void* systemData, void* userData);
		virtual void StartFeature();
		virtual void StopFeature();

		static int32 CameraUpdate(void* systemData, void* userData);
		void CloseFeed();
		void OpenFeed();
	};

	class CsmCameraImage: public CsmImageSource
	{
	};
}