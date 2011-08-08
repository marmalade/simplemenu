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
	position.x = DPI::CdpiLength(IW_GEOM_ONE/2, DPI::CdpiLength::PERCENT);
	position.y = DPI::CdpiLength(IW_GEOM_ONE/2, DPI::CdpiLength::PERCENT);
	pivot.x = DPI::CdpiLength(IW_GEOM_ONE/2, DPI::CdpiLength::PERCENT);
	pivot.y = DPI::CdpiLength(IW_GEOM_ONE/2, DPI::CdpiLength::PERCENT);
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
void CsmFloatingPanel::PrepareChildItems(smItemContext* context, const CIwSVec2& recommendedSize)
{
	int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = recommendedSize.x;
	size.y = 0;
	CIwSVec2 chRecSize (contentWidth,recommendedSize.y);
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(context,chRecSize);
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
	if (!stricmp("pivot",pAttrName))
	{
		pivot.ParseAttribute(pParser);
		return true;
	}
	if (!stricmp("position",pAttrName))
	{
		position.ParseAttribute(pParser);
		return true;
	}
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
