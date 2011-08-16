#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "smItem.h"

namespace SimpleMenu
{
	class CsmTerminalItem : public CsmItem
	{
	public:
		//Constructor
		CsmTerminalItem();
		//Desctructor
		virtual ~CsmTerminalItem();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
	};
}