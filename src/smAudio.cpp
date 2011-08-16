#include "smAudio.h"
#include "s3eAudio.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmAudio* g_smAudio = 0;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmAudio::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmAudio> d (0, "CsmAudio",
		ScriptTraits::Method("IsAvailable", &CsmAudio::IsAvailable),
		ScriptTraits::Method("PlayFile", &CsmAudio::PlayFile),
			0);
	return &d;
}
bool CsmAudio::IsAvailable()
{
	return s3eAudioGetInt(S3E_AUDIO_AVAILABLE) != 0;
}
bool CsmAudio::PlayFile(const char* fileName, int repeatCount)
{
	return S3E_RESULT_SUCCESS == s3eAudioPlay(fileName, (uint32)repeatCount);
}
