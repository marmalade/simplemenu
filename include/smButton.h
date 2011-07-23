#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "smClickable.h"

namespace SimpleMenu
{
	class CsmButton : public CsmClickable
	{
	private:
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmButton);
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
		//Constructor
		CsmButton();
		//Desctructor
		virtual ~CsmButton();

		virtual uint32 GetElementNameHash();
	};
}