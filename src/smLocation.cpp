#include "smLocation.h"
//#include "smConfig.h"

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
		//CsmConfig::SetFloat("sm-location-latitude", (float)l.m_Latitude);
		//CsmConfig::SetFloat("sm-location-longitude", (float)l.m_Longitude);
}
void CsmLocation::LoadFromConfig()
{
		//l.m_Latitude = CsmConfig::GetFloat("sm-location-latitude");
		//l.m_Longitude = CsmConfig::GetFloat("sm-location-longitude");
}
void CsmLocation::StartFeature()
{
	m_recievedAt = 0;
	s3eLocationStart();
	//if (CsmConfig::IsExist("sm-location-latitude"))
	//{
	//	g_smLocation->LoadFromConfig();
	//}
	//else
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
		g_smLocation->m_recievedAt = time(0);
		g_smLocation->SaveToConfig();
	}
	return 0;
}
void CsmLocation::StopFeature()
{
	s3eLocationStop();
	s3eLocationUnRegister(S3E_LOCATION_CALLBACK_LOCATION_UPDATED, Callback);
}
float CsmLocation::GetLatitude()
{
	return (float)RequestFeature()->l.m_Latitude;
}
float CsmLocation::GetLongitude()
{
	return (float)RequestFeature()->l.m_Latitude;
}
