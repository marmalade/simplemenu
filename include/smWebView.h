#pragma once
//#include <s3eWebView.h>
#include "smFeatures.h"

namespace SimpleMenu
{
	class CsmWebView: public CsmFeature
	{
	public: 
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();

		static bool IsAvailable();
		static void OpenWebPage(const char*);
	};
}