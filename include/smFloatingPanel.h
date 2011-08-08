#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "string"
#include "smItem.h"
#include "smMenu.h"

namespace SimpleMenu
{
	class CsmFloatingPanel : public CsmItem
	{
	protected:
		DPI::CdpiLength2 pivot;
		DPI::CdpiLength2 position;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmFloatingPanel);
		//Constructor
		CsmFloatingPanel();
		//Desctructor
		virtual ~CsmFloatingPanel();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		virtual void PrepareChildItems(smItemContext* context, const CIwSVec2& recommendedSize);
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