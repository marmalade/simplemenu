#include <iwgx.h>
#include "smWebView.h"
#include "s3eOSExec.h"
#include "s3eWebView.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmWebView* g_smWebVideo = 0;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmWebView::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmWebView> d (0, "CsmWebView",
		ScriptTraits::Method("IsAvailable", &CsmWebView::IsAvailable),
		ScriptTraits::Method("OpenWebPage", &CsmWebView::OpenWebPage),
			0);
	return &d;
}
bool CsmWebView::IsAvailable()
{
	return s3eWebViewAvailable() || s3eOSExecAvailable();
}
void CsmWebView::OpenWebPage(const char* url)
{
	if (IsAvailable())
	{
//		return;
	}
	if (s3eOSExecAvailable())
	{
		s3eOSExecExecute (url, false);
	}
}