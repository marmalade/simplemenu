#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include <smStyle.h>

#define SM_ANYSTYLE (0x0002b5cf)

namespace SimpleMenu
{
	struct smStyleSheetItem
	{
		uint32 NameHash;
		uint32 ClassHash;
		uint32 StateHash;
		uint32 Index;
	};
	class CsmStyleSheet : public CIwResource
	{
	protected:
		CIwManagedList childItems;
		CIwArray<smStyleSheetItem> map;
		CIwArray<CsmStyle> styles;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmStyleSheet);
		//Constructor
		CsmStyleSheet();
		//Desctructor
		virtual ~CsmStyleSheet();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		void Apply(CsmStyleSettings* other, uint32,uint32,uint32);

#ifdef IW_BUILD_RESOURCES
		//Parses from text file: start block.
		virtual	void	ParseOpen(CIwTextParserITX* pParser);
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
		//Parses from text file: end block.
		virtual	void	ParseClose(CIwTextParserITX* pParser);
		//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
		virtual	void	ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild);
#endif
	};

#ifdef IW_BUILD_RESOURCES
	class CsmStyleSheetResHandler : public CIwResHandler
	{
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmStyleSheetResHandler);

		//Constructor
		CsmStyleSheetResHandler();
		//Desctructor
		virtual ~CsmStyleSheetResHandler();

		//Buid resource method
		virtual CIwResource*	Build(const CIwStringL& pathname);
	};
#endif
}