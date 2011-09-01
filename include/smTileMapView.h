#pragma once

namespace SimpleMenu
{
	class CsmTileMapView
	{
	public:
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
	};
}