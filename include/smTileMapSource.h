#pragma once

#include <IwManaged.h>
#include <smMenu.h>

namespace SimpleMenu
{
	class CsmTileMapSource: public CIwManaged
	{
	public:
		//Constructor
		CsmTileMapSource();
		//Desctructor
		virtual ~CsmTileMapSource();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
	};
}