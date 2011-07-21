#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smStyleSheet.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmStyleSheet);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmStyleSheet);

//Constructor
CsmStyleSheet::CsmStyleSheet()
{
}
//Desctructor
CsmStyleSheet::~CsmStyleSheet()
{
	childItems.Delete();
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmStyleSheet::Serialise ()
{
	CIwResource::Serialise();
	childItems.Serialise();
	map.SerialiseHeader();
	for (uint32 i=0; i<map.size(); ++i)
	{
		IwSerialiseUInt32(map[i].NameHash);
		IwSerialiseUInt32(map[i].ClassHash);
		IwSerialiseUInt32(map[i].StateHash);
		IwSerialiseUInt32(map[i].Index);
	}
	styles.SerialiseHeader();
	for (uint32 i=0; i<styles.size(); ++i)
	{
		styles[i].Serialise();
	}
}

void CsmStyleSheet::Apply(CsmStyleSettings* other, uint32 n, uint32 c, uint32 s)
{
	for (uint32 i=0; i<map.size(); ++i)
	{
		smStyleSheetItem& st = map[i];
		if (((st.ClassHash == c) || (st.ClassHash == SM_ANYSTYLE)) &&
			((st.NameHash == n) || (st.NameHash == SM_ANYSTYLE)) &&
			((st.StateHash == s) || (st.StateHash == SM_ANYSTYLE)))
		{
			styles[map[i].Index].Apply(other);
		}
	}
}

#ifdef IW_BUILD_RESOURCES
//Parses from text file: start block.
void	CsmStyleSheet::ParseOpen(CIwTextParserITX* pParser)
{
	CIwResource::ParseOpen(pParser);
}
//Parses from text file: parses attribute/value pair.
bool	CsmStyleSheet::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("bindto", pAttrName))
	{
		map.push_back();
		pParser->ReadStringHash(&map.back().NameHash);
		pParser->ReadStringHash(&map.back().ClassHash);
		pParser->ReadStringHash(&map.back().StateHash);
		map.back().Index = styles.size();
		return true;
	}
	if (!stricmp("style", pAttrName))
	{
		styles.push_back();
		pParser->PushObject(&styles.back());
		return true;
	}
	return CIwResource::ParseAttribute(pParser, pAttrName);
}
//Parses from text file: end block.
void	CsmStyleSheet::ParseClose(CIwTextParserITX* pParser)
{
	IwGetResManager()->GetCurrentGroup()->AddRes(IwHashString("CsmStyleSheet"), this);
}
//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
void	CsmStyleSheet::ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild)
{
	if (pChild == ((CIwManaged*)&styles.back()))
		return;
	//CIwResource::ParseCloseChild(pParser, pChild);
	childItems.Add(pChild);
}

IW_MANAGED_IMPLEMENT(CsmStyleSheetResHandler)

//Constructor
CsmStyleSheetResHandler::CsmStyleSheetResHandler(): CIwResHandler("simplemenustyle", "CsmStyleSheet")
{
	SetName("smSTYLE");
}
//Desctructor
CsmStyleSheetResHandler::~CsmStyleSheetResHandler()
{
}

//Buid resource method
CIwResource*	CsmStyleSheetResHandler::Build(const CIwStringL& pathname)
{
	IwGetTextParserITX()->ParseFile(pathname.c_str());
	return NULL;
}

#endif