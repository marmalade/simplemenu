#include <iwgx.h>
#include "smVideo.h"
#include "s3eVideo.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmVideo* g_smVideo = 0;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmVideo::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmVideo> d ("CsmVideo",
		ScriptTraits::Method("IsAvailable", &CsmVideo::IsAvailable),
		ScriptTraits::Method("PlayFile", &CsmVideo::PlayFile),
			0);
	return &d;
}
bool CsmVideo::IsAvailable()
{
	return s3eVideoGetInt(S3E_VIDEO_AVAILABLE) != 0;
}
bool CsmVideo::PlayFile(const char* fileName, int repeatCount)
{
	return S3E_RESULT_SUCCESS == s3eVideoPlay(fileName, (uint32)repeatCount,0,0,IwGxGetScreenWidth(),IwGxGetScreenHeight());
}
