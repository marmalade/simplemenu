#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smCanvas.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmCanvas);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmCanvas);

//Constructor
CsmCanvas::CsmCanvas()
{

}
//Desctructor
CsmCanvas::~CsmCanvas()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCanvas::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCanvas> d ("CsmCanvas",
			ScriptTraits::Method("GetRoot", &CsmCanvas::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmCanvas::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmCanvas::GetChildItemsCount),
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmCanvas::Serialise ()
{
	CsmItem::Serialise();
}
uint32 CsmCanvas::GetElementNameHash()
{
	static uint32 name = IwHashString("Canvas");
	return name;
}
void CsmCanvas::PrepareChildItems(smItemContext* context, const CIwSVec2& recommendedSize)
{
	int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = recommendedSize.x;
	size.y = 0;
	CIwSVec2 chRecSize (contentWidth, recommendedSize.y);
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
void CsmCanvas::RearrangeChildItems()
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
void CsmCanvas::Render(smItemContext* renderContext)
{
	CsmItem::Render(renderContext);
}


#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmCanvas::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
