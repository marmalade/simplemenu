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
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmCameraImage);
		//Constructor
		CsmCameraImage();
		//Desctructor
		virtual ~CsmCameraImage();
		//Reads/writes a binary file using @a IwSerialise interface.
		void Serialise ();

		// Check if imge is available (loaded, downloaded etc)
		virtual bool IsAvailable() const;
		// Get recommended size of the image
		virtual CIwSVec2 GetRecommendedSize(const CIwSVec2& area) const;
		// Prepare image source to render the image
		virtual void Prepare(const CIwSVec2& recommendedSize);
		// Get image material to draw quad
		virtual CIwMaterial* GetMaterial();
		//Animate item and all child items
		virtual void Animate(iwfixed timespan);
	};
}