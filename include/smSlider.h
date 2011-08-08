#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "string"
#include "smItem.h"
#include "smMenu.h"

namespace SimpleMenu
{
	class CsmSlider : public CsmItem
	{
	protected:
		iwfixed sliderWidth;
		iwfixed sliderValue;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmSlider);
		//Constructor
		CsmSlider();
		//Desctructor
		virtual ~CsmSlider();

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
		//Check if element can interact with user
		virtual bool IsActive() const {return true;}

		virtual void TouchMotion(smTouchContext* smTouchContext);
		virtual bool KeyReleasedEvent(smKeyContext* keyContext);
		virtual bool KeyPressedEvent(smKeyContext* keyContext);
	protected:
		virtual void ApplyChildStyle(smItemContext* renderContext, CsmItem*child);
	public:
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}