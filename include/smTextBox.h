#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "smTerminalItem.h"

namespace SimpleMenu
{
	class CsmTextBox : public CsmTerminalItem
	{
	protected:
		std::string onChanged;
		char* utf8string;
		CsmStyleSettings cachedWithCombinedStyle;
		int16 cachedWithWidth;
		FreeTypeHelper::CfthGlyphLayoutData layoutData;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmTextBox);
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
		//Constructor
		CsmTextBox();
		//Constructor
		CsmTextBox(char* buf);
		//Desctructor
		virtual ~CsmTextBox();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
		//Gets the element name hash to select an apropriate style
		virtual uint32 GetElementNameHash();

		//Check if element can interact with user (receive clicks and keys)
		virtual bool IsActive() const {return true;}
		virtual void TouchReleased(smTouchContext* touchContext);

		virtual void Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize);
		//Render image on the screen surface
		virtual void Render(smItemContext* renderContext);
		virtual void RearrangeChildItems();

		const char* GetText() const;
		void SetText(const char*);

		void OnTextChanged();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}