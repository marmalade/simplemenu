#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "string"
#include "smItem.h"
#include "smMenu.h"

namespace SimpleMenu
{
	class CsmCanvas : public CsmItem
	{
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmCanvas);
		//Constructor
		CsmCanvas();
		//Desctructor
		virtual ~CsmCanvas();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		virtual void PrepareChildItems(smItemContext* context,int16 width);
		virtual void RearrangeChildItems();
		//Render image on the screen surface
		virtual void Render(smItemContext* renderContext);

		//Gets the element name hash to select an apropriate style
		virtual uint32 GetElementNameHash();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}