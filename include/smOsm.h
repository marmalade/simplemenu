#pragma once

#include "smScriptProvider.h"

namespace SimpleMenu
{
	void smOsmInit();
	void smOsmTerminate();

	class CsmOsm
	{
	public:
		static float GetDistance(float lon1, float lat1, float lon2, float lat2);

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
	};
}