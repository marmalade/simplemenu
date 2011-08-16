#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smTerminalItem.h"
#include "smTextBlock.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmTerminalItem::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmTerminalItem> d (CsmItem::GetClassDescription(), "CsmTerminalItem",
			0);
	return &d;
}

//Constructor
CsmTerminalItem::CsmTerminalItem()
{

}
//Desctructor
CsmTerminalItem::~CsmTerminalItem()
{
	
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmTerminalItem::Serialise ()
{
	CsmItem::Serialise();
	
}
