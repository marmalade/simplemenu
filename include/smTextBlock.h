#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "smTerminalItem.h"

namespace SimpleMenu
{
	class CsmTextBlock : public CsmTerminalItem
	{
	private:
		char* utf8string;
		CsmStyleSettings cachedWithCombinedStyle;
		int16 cachedWithWidth;
		FreeTypeHelper::CfthGlyphLayoutData layoutData;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmTextBlock);
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
		//Constructor
		CsmTextBlock();
		//Constructor
		CsmTextBlock(char* buf);
		//Desctructor
		virtual ~CsmTextBlock();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		virtual void Prepare(smItemContext* renderContext,int16 width);
		//Render image on the screen surface
		virtual void Render(smItemContext* renderContext);
		virtual void RearrangeChildItems();

		const char* GetText() const;
		void SetText(const char*);
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}