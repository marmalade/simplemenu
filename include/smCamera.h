#pragma once
#include <s3eCamera.h>
#include <smImageSource.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmCamera: public CsmFeature
	{
	protected:
		volatile bool feedUpdated;
		bool started;

		CIwImage* image;
		int16* activeBuffer;
		int16* completeBuffer;
		CIwTexture* texture;
		int32 feedWidth;
		int32 feedHeight;

		s3eCameraStreamingSizeHint sizeHint;
		int quality;
		int type;
		int contrast;
		int brightness;
		int focus;
	public: 
		CsmCamera();
		virtual ~CsmCamera();
		//Is feature available on device
		static bool IsAvailable();
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();

		static CsmCamera* RequestFeature();
		CIwTexture* GetTexture();
		static void SetQualityLow();
		static void SetQualityMedium();
		static void SetQualityHigh();
		static void SetSizeSmallest();
		static void SetSizeMedium();
		static void SetSizeLargest();
		void SetQuality(int q);
		void SetSize(s3eCameraStreamingSizeHint s);
	protected:
		//static int32 Callback (void* systemData, void* userData);
		virtual void StartFeature();
		virtual void StopFeature();

		static int32 CameraUpdate(void* systemData, void* userData);

		void RequireImage(int16 reqw, int16 reqh);
		bool FlipCoordinates() const {return true;};

		void CloseFeed();
		void OpenFeed();
	};

	class CsmCameraImage: public CsmImageSource
	{
		CIwMaterial*material;
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