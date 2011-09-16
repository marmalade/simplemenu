//#include <s3eKeyboard.h>
//#include <sstream>
//#include "smLoadingScreen.h"
//
//
//using namespace SimpleMenu;
//
//namespace SimpleMenu
//{
//	CsmLoadingScreen* g_smLoadingScreen = 0;
//
//}
//
////Get scriptable class declaration
//CsmScriptableClassDeclaration* CsmLoadingScreen::GetClassDescription()
//{
//	static  TsmScriptableClassDeclaration<CsmLoadingScreen> d (0, "CsmLoadingScreen",
//		ScriptTraits::Method("IsAvailable", &CsmLoadingScreen::IsAvailable),
//		ScriptTraits::Method("Enable", &CsmLoadingScreen::Enable),
//		ScriptTraits::Method("Disable", &CsmLoadingScreen::Disable),
//			0);
//	return &d;
//}
//bool CsmLoadingScreen::IsAvailable()
//{
//	return true;
//}
//CsmLoadingScreen::CsmLoadingScreen()
//{
//	counter = 0;
//}
//CsmLoadingScreen::~CsmLoadingScreen()
//{
//	g_smLoadingScreen = 0;
//}
//void CsmLoadingScreen::Enable()
//{
//	CsmLoadingScreen* feature = RequestFeature();
//	if (!feature)
//		return;
//	if (!feature->counter)
//	{
//
//	}
//	++feature->counter;
//}
//void CsmLoadingScreen::Disable()
//{
//	CsmLoadingScreen* feature = RequestFeature();
//	if (!feature)
//		return;
//	--feature->counter;
//	if (!feature->counter)
//	{
//	}
//}
//CsmLoadingScreen* CsmLoadingScreen::RequestFeature()
//{
//	if (g_smLoadingScreen)
//		return g_smLoadingScreen;
//	g_smLoadingScreen = new CsmLoadingScreen();
//	g_smLoadingScreen->Start();
//	return g_smLoadingScreen;
//}
//int32 Callback (void* systemData, void* userData)
//{
//}
//void CsmLoadingScreen::StartFeature()
//{
//}
//void CsmLoadingScreen::StopFeature()
//{
//}
