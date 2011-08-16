#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smButton.h"
#include "smTextBlock.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmButton);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmButton);

//Constructor
CsmButton::CsmButton()
{

}
//Desctructor
CsmButton::~CsmButton()
{
}
uint32 CsmButton::GetElementNameHash()
{
	static uint32 name = IwHashString("BUTTON");
	return name;
}
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmButton::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmButton> d (CsmItem::GetClassDescription(), "CsmButton",
			ScriptTraits::Method("GetRoot", &CsmButton::GetRoot),
			ScriptTraits::Method("GetParent", &CsmButton::GetParent),
			ScriptTraits::Method("AddTextBlock", &CsmButton::AddTextBlock),
			ScriptTraits::Method("GetChildAt", &CsmButton::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmButton::GetChildItemsCount),
			0);
	return &d;
}