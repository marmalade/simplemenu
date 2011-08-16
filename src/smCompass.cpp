#include "smCompass.h"
#include "s3eCompass.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmCompass* g_smCompass = 0;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCompass::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCompass> d (0, "CsmCompass",
		ScriptTraits::Method("IsAvailable", &CsmCompass::IsAvailable),
		ScriptTraits::Method("GetX", &CsmCompass::GetX),
		ScriptTraits::Method("GetY", &CsmCompass::GetY),
		ScriptTraits::Method("GetZ", &CsmCompass::GetZ),
			0);
	return &d;
}
CsmCompass::~CsmCompass()
{
	g_smCompass = 0;
}
CsmCompass* CsmCompass::RequestFeature()
{
	if (g_smCompass)
		return g_smCompass;
	if (!IsAvailable())
		return 0;
	g_smCompass = new CsmCompass();
	g_smCompass->Start();
	return g_smCompass;
}
bool CsmCompass::IsAvailable()
{
	return s3eCompassAvailable() != 0;
}
void CsmCompass::StartFeature()
{
	s3eCompassStart();
}
void CsmCompass::StopFeature()
{
	s3eCompassStop();
}
int CsmCompass::GetX()
{
	if (RequestFeature())
	{
		s3eCompassHeading dir;
		s3eCompassGetHeading(&dir);
		return dir.m_X;
	}
	return 0;
}
int CsmCompass::GetY()
{
	if (RequestFeature())
	{
		s3eCompassHeading dir;
		s3eCompassGetHeading(&dir);
		return dir.m_Y;
	}
	return 0;
}
int CsmCompass::GetZ()
{
	if (RequestFeature())
	{
		s3eCompassHeading dir;
		s3eCompassGetHeading(&dir);
		return dir.m_Z;
	}
	return 0;
}
