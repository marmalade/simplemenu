#pragma once
#include <s3eLocation.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmLocation: public CsmFeature
	{

	public: 
		private:
		s3eLocation l;
		time_t m_recievedAt;

	public: 
		virtual ~CsmLocation();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		static bool IsAvailable();
		static float GetLatitude();
		static float GetLongitude();
		static float GetAltitude();
		static float GetHorizontalAccuracy();
		static float GetTimeStampUTC();
		static float GetVerticalAccuracy();
		static long GetDataTimestamp();
	protected:
		static CsmLocation* RequestFeature();
		static int32 Callback (void* systemData, void* userData);
		virtual void StartFeature();
		virtual void StopFeature();
		void SaveToConfig();
		void LoadFromConfig();

	};
}