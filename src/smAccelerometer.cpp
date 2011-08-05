#include "smAccelerometer.h"
#include "s3eAccelerometer.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmAccelerometer* g_smAccelerometer = 0;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmAccelerometer::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmAccelerometer> d ("CsmAccelerometer",
		ScriptTraits::Method("IsAvailable", &CsmAccelerometer::IsAvailable),
		ScriptTraits::Method("GetX", &CsmAccelerometer::GetX),
		ScriptTraits::Method("GetY", &CsmAccelerometer::GetY),
		ScriptTraits::Method("GetZ", &CsmAccelerometer::GetZ),
			0);
	return &d;
}

bool CsmAccelerometer::IsAvailable()
{
	return s3eAccelerometerGetInt( S3E_ACCELEROMETER_AVAILABLE ) != 0;
}
CsmAccelerometer::~CsmAccelerometer()
{
	g_smAccelerometer = 0;
}
CsmAccelerometer* CsmAccelerometer::RequestFeature()
{
	if (g_smAccelerometer)
		return g_smAccelerometer;
	if (!IsAvailable())
		return 0;
	g_smAccelerometer = new CsmAccelerometer();
	g_smAccelerometer->Start();
	return g_smAccelerometer;
}
void CsmAccelerometer::StartFeature()
{
	s3eAccelerometerStart();
}
void CsmAccelerometer::StopFeature()
{
	s3eAccelerometerStop();
}
int CsmAccelerometer::GetX()
{
	if (RequestFeature())
		return s3eAccelerometerGetX();
	return 0;
}
int CsmAccelerometer::GetY()
{
	if (RequestFeature())
		return s3eAccelerometerGetY();
	return 0;
}
int CsmAccelerometer::GetZ()
{
	if (RequestFeature())
		return s3eAccelerometerGetZ();
	return 1000;
}
