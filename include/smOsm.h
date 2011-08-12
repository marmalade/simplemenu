#pragma once

#include "smScriptProvider.h"

namespace SimpleMenu
{
	void smOsmInit();
	void smOsmTerminate();

	class CsmOsm
	{
	public:
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
	};
}