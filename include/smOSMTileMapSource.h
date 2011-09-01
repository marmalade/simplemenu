#pragma once

#include <smTileMapSource.h>

namespace SimpleMenu
{
	class CsmOSMTileMapSource: public CsmTileMapSource
	{
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmOSMTileMapSource);
		//Constructor
		CsmOSMTileMapSource();
		//Desctructor
		virtual ~CsmOSMTileMapSource();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
	};
}