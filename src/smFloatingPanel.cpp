#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smFloatingPanel.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmFloatingPanel);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmFloatingPanel);

//Constructor
CsmFloatingPanel::CsmFloatingPanel()
{

}
//Desctructor
CsmFloatingPanel::~CsmFloatingPanel()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmFloatingPanel::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmFloatingPanel> d ("CsmFloatingPanel",
			ScriptTraits::Method("GetRoot", &CsmFloatingPanel::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmFloatingPanel::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmFloatingPanel::GetChildItemsCount),
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmFloatingPanel::Serialise ()
{
	CsmItem::Serialise();
}
uint32 CsmFloatingPanel::GetElementNameHash()
{
	static uint32 name = IwHashString("FloatingPanel");
	return name;
}
void CsmFloatingPanel::PrepareChildItems(smItemContext* context,int16 width)
{
	int16 contentWidth = width - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = width;
	size.y = 0;
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(context,contentWidth);
		int32 height = item->GetSize().y;
		if (size.y < height)
		size.y = height;
	}
	size.y += GetContentOffsetTop()+GetContentOffsetBottom();
}
void CsmFloatingPanel::RearrangeChildItems()
{
	CIwSVec2 topLeft = GetOrigin();
	topLeft.x += GetContentOffsetLeft();
	topLeft.y += GetContentOffsetTop();
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->SetOrigin(topLeft);
	}
}

//Render image on the screen surface
void CsmFloatingPanel::Render(smItemContext* renderContext)
{
	CsmItem::Render(renderContext);
}


#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmFloatingPanel::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
