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

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
	};
}