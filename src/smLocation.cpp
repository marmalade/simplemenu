#include <s3eKeyboard.h>
#include <sstream>
#include "smLocation.h"
#include "smConfig.h"
#include "smWindowHistory.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmLocation* g_smLocation = 0;

	bool smWaitForGPSCallback(CsmMenu*m, void*arg)
	{
		time_t* prevTime = (time_t*) arg;
		time_t t = CsmLocation::GetDataTimestamp();
		return (t == *prevTime);
	}
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmLocation::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmLocation> d ("CsmLocation",
		ScriptTraits::Method("IsAvailable", &CsmLocation::IsAvailable),
		ScriptTraits::Method("GetLatitude", &CsmLocation::GetLatitude),
		ScriptTraits::Method("GetLongitude", &CsmLocation::GetLongitude),
		ScriptTraits::Method("GetAltitude", &CsmLocation::GetAltitude),
		ScriptTraits::Method("GetHorizontalAccuracy", &CsmLocation::GetHorizontalAccuracy),
		ScriptTraits::Method("GetTimeStampUTC", &CsmLocation::GetTimeStampUTC),
		ScriptTraits::Method("GetVerticalAccuracy", &CsmLocation::GetVerticalAccuracy),
		ScriptTraits::Method("GetDataTimestamp", &CsmLocation::GetDataTimestamp),
		ScriptTraits::Method("WaitForGPS", &CsmLocation::WaitForGPS),
		ScriptTraits::Method("GetNumSatellitesInView", &CsmLocation::GetNumSatellitesInView),
		ScriptTraits::Method("GetNumSatellitesUsed", &CsmLocation::GetNumSatellitesUsed),
		ScriptTraits::Method("GetSourceName", &CsmLocation::GetSourceName),
			0);
	return &d;
}

bool CsmLocation::IsAvailable()
{
	return s3eLocationAvailable() != 0;
}
CsmLocation::~CsmLocation()
{
	g_smLocation = 0;
}
CsmLocation* CsmLocation::RequestFeature()
{
	if (g_smLocation)
		return g_smLocation;
	g_smLocation = new CsmLocation();
	g_smLocation->Start();
	return g_smLocation;
}
void CsmLocation::SaveToConfig()
{
	CsmConfig::SetFloat("sm-location-latitude", (float)l.m_Latitude);
	CsmConfig::SetFloat("sm-location-longitude", (float)l.m_Longitude);
	CsmConfig::SetInteger("sm-location-m_received-at", (int)m_receivedAt);
	
}
void CsmLocation::LoadFromConfig()
{
	l.m_Latitude = CsmConfig::GetFloat("sm-location-latitude");
	l.m_Longitude = CsmConfig::GetFloat("sm-location-longitude");
	m_receivedAt = (time_t)CsmConfig::GetInteger("sm-location-m_received-at");
}
void CsmLocation::StartFeature()
{
	m_receivedAt = 0;
	if (S3E_RESULT_SUCCESS != s3eLocationRegister(S3E_LOCATION_CALLBACK_LOCATION_UPDATED, Callback, this))
	{
		smAlert("GPS Register Error!", s3eLocationGetErrorString());
	}
	if (S3E_RESULT_SUCCESS != s3eLocationStart())
	{
		smAlert("GPS Start Error!", s3eLocationGetErrorString());
	}
	if (CsmConfig::IsExist("sm-location-latitude"))
	{
		g_smLocation->LoadFromConfig();
	}
	else
	{
		if (S3E_RESULT_SUCCESS == s3eLocationGet(&l))
		{
			SaveToConfig();
		}
	}
}
int32 CsmLocation::Callback (void* systemData, void* userData)
{
	if (g_smLocation)
	{
		g_smLocation->l = *((s3eLocation*)systemData);
		g_smLocation->m_receivedAt = time(0);
		g_smLocation->SaveToConfig();
	}
	return 0;
}
void CsmLocation::Update()
{
	s3eLocation newl;
	if (S3E_RESULT_SUCCESS == s3eLocationGet(&newl))
	{
		if (newl.m_Latitude != l.m_Latitude || 
			newl.m_Longitude != l.m_Longitude)
		{
			Callback(&newl, 0);
		}
	}
}
void CsmLocation::StopFeature()
{
	if (S3E_RESULT_SUCCESS != s3eLocationStop())
	{
		smAlert("GPS Stop Error!", s3eLocationGetErrorString());
	}
	if (S3E_RESULT_SUCCESS != s3eLocationUnRegister(S3E_LOCATION_CALLBACK_LOCATION_UPDATED, Callback))
	{
		smAlert("GPS Unregister Error!", s3eLocationGetErrorString());
	}
}
bool CsmLocation::WaitForGPS()
{
	CsmLocation* feature = RequestFeature();
	if (!feature)
		return false;

	time_t r = feature->m_receivedAt;

	smOpenWaitDialog("GPS","Waiting for GPS satellite signal...", smWaitForGPSCallback, &r);

	return (feature->m_receivedAt != r);
}
float CsmLocation::GetLatitude()
{
	return (float)RequestFeature()->l.m_Latitude;
}
float CsmLocation::GetLongitude()
{
	return (float)RequestFeature()->l.m_Longitude;
}
float CsmLocation::GetAltitude()
{
	return (float)RequestFeature()->l.m_Altitude;
}
float CsmLocation::GetHorizontalAccuracy()
{
	return (float)RequestFeature()->l.m_HorizontalAccuracy;
}
float CsmLocation::GetTimeStampUTC()
{
	return (float)RequestFeature()->l.m_TimeStampUTC;
}
float CsmLocation::GetVerticalAccuracy()
{
	return (float)RequestFeature()->l.m_VerticalAccuracy;
}
long CsmLocation::GetDataTimestamp()
{
	return (long)RequestFeature()->m_receivedAt;
}
bool CsmLocation::GetGPSData(s3eLocationGPSData*data)
{
	if (!RequestFeature())
		return false;

	int t = s3eLocationGetInt(S3E_LOCATION_TYPE);
	if (t != S3E_LOCATION_GPS && t != S3E_LOCATION_HYBRID)
		return false;

	if (S3E_RESULT_SUCCESS == s3eLocationGetGPSData (data))
	{
		return true;
	}
	int errCode = s3eLocationGetError();
	switch (errCode)
	{
	case S3E_LOCATION_ERR_UNSUPPORTED:
		break;
	case S3E_LOCATION_ERR_UNAVAIL:
		break;
	case S3E_LOCATION_ERR_NONE:
		break;
	default:
		const char* err = s3eLocationGetErrorString();
		std::stringstream s; s << errCode << ": " << err;
		smAlert("GPS Data Error!", s.str().c_str());
		break;
	}

	return false;
}

int CsmLocation::GetNumSatellitesInView()
{
	s3eLocationGPSData data;
	if (GetGPSData(&data))
		return data.m_NumSatellitesInView;
	return -1;
}
const char* CsmLocation::GetSourceName()
{
	if (!RequestFeature())
		return "";
	int t = s3eLocationGetInt(S3E_LOCATION_TYPE);
	switch (t)
	{
	case -1:
		return "";
	case S3E_LOCATION_GPS:
		return "GPS";
	case S3E_LOCATION_WIFI:
		return "WiFi";
	case S3E_LOCATION_CELLTOWER:
		return "Cell Tower";
	case S3E_LOCATION_HYBRID:
		return "Hybrid";
	case S3E_LOCATION_UNKNOWN:
		return "Unknown";
	case S3E_LOCATION_NONE:
	default:	
		return "";
	}
}
int CsmLocation::GetNumSatellitesUsed()
{
		s3eLocationGPSData data;
	if (GetGPSData(&data))
		return data.m_NumSatellitesUsed;
	return -1;

}

