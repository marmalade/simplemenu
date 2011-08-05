#include "smCamera.h"
#include "s3eCamera.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmCamera* g_smCamera = 0;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCamera::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCamera> d ("CsmCamera",
		ScriptTraits::Method("IsAvailable", &CsmCamera::IsAvailable),
		
			0);
	return &d;
}
CsmCamera::CsmCamera()
{
	//image = 0;
	//texture = 0;
	//activeBuffer= 0;
	//completeBuffer = 0;
	feedUpdated = false;
	started = false;
}
CsmCamera::~CsmCamera()
{
	g_smCamera = 0;
}
CsmCamera* CsmCamera::RequestFeature()
{
	if (g_smCamera)
		return g_smCamera;
	if (!IsAvailable())
		return 0;
	g_smCamera = new CsmCamera();
	g_smCamera->Start();
	return g_smCamera;
}
bool CsmCamera::IsAvailable()
{
	return s3eCameraAvailable() != 0;
}
void CsmCamera::StartFeature()
{
	OpenFeed();
}
void CsmCamera::StopFeature()
{
}
void CsmCamera::CloseFeed()
{
	if (!started)
		return;
	s3eCameraStop();
	s3eCameraUnRegister(S3E_CAMERA_UPDATE_STREAMING, CameraUpdate);
	started = false;
}
void CsmCamera::OpenFeed()
{
	if (started)
		return;
	if (S3E_RESULT_ERROR == s3eCameraRegister(S3E_CAMERA_UPDATE_STREAMING, CameraUpdate, this))
	{
		return;
	}
	if (s3eCameraStart(S3E_CAMERA_STREAMING_SIZE_HINT_SMALLEST, S3E_CAMERA_PIXEL_TYPE_RGB565_CONVERTED) == S3E_RESULT_SUCCESS)
	{
		started = true;
	}
}

// Callback that gets filled with camera buffer data
int32 CsmCamera::CameraUpdate(void* systemData, void* userData)
{
	CsmCamera* g_currentFeed = (CsmCamera*)userData;
	if (!g_currentFeed)
		return 0;
	if (g_currentFeed->feedUpdated)
		return 0;
}