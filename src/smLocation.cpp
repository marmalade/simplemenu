#include <s3eKeyboard.h>
#include "smLocation.h"
#include "smConfig.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmLocation* g_smLocation = 0;
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
	CsmConfig::SetLong("sm-location-m_received-at", (long)m_receivedAt);
}
void CsmLocation::LoadFromConfig()
{
	l.m_Latitude = CsmConfig::GetFloat("sm-location-latitude");
	l.m_Longitude = CsmConfig::GetFloat("sm-location-longitude");
	m_receivedAt = CsmConfig::GetLong("sm-location-m_received-at");
}
void CsmLocation::StartFeature()
{
	m_receivedAt = 0;
	s3eLocationStart();
	if (CsmConfig::IsExist("sm-location-latitude"))
	{
		g_smLocation->LoadFromConfig();
	}
	else
	{
		s3eLocationGet(&l);
		SaveToConfig();
	}
	s3eLocationRegister(S3E_LOCATION_CALLBACK_LOCATION_UPDATED, Callback, this);
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
void CsmLocation::StopFeature()
{
	s3eLocationStop();
	s3eLocationUnRegister(S3E_LOCATION_CALLBACK_LOCATION_UPDATED, Callback);
}
bool CsmLocation::WaitForGPS()
{
	CsmLocation* feature = RequestFeature();
	if (!feature)
		return false;

	time_t r = feature->m_receivedAt;
	time_t rr = r;
	while (rr == r)
	{
		s3eDeviceYield(30);
		s3eKeyboardUpdate();

		bool result = true;
		if	(
			(result == false) ||
			(s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN) ||
			(s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN) ||
			(s3eDeviceCheckQuitRequest()) ||
			(SimpleMenu::smGetCloseState() != SimpleMenu::SM_KEEP_OPEN)
			)
			break;

		smRenderLoading();
		rr = feature->m_receivedAt;
	}
	while ((s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN))
	{
		s3eDeviceYield(30);
		s3eKeyboardUpdate();
	}
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