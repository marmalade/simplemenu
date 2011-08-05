#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smRow.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmRow);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmRow);

//Constructor
CsmRow::CsmRow()
{

}
//Desctructor
CsmRow::~CsmRow()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmRow::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmRow> d ("CsmRow",
			ScriptTraits::Method("GetRoot", &CsmRow::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmRow::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmRow::GetChildItemsCount),
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmRow::Serialise ()
{
	CsmItem::Serialise();
}
uint32 CsmRow::GetElementNameHash()
{
	static uint32 name = IwHashString("Row");
	return name;
}
void CsmRow::PrepareChildItems(smItemContext* context,int16 width)
{
	if (childItems.GetSize() == 0)
		return;
	int16 contentWidth = width - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = width;
	size.y = 0;
	int16 cellWidth = contentWidth/childItems.GetSize();
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(context,cellWidth);
		int32 height = item->GetSize().y;
		if (size.y < height)
		size.y = height;
	}
	size.y += GetContentOffsetTop()+GetContentOffsetBottom();
}
void CsmRow::RearrangeChildItems()
{
	if (childItems.GetSize() == 0)
		return;
	CIwSVec2 topLeft = GetOrigin();
	topLeft.x += GetContentOffsetLeft();
	topLeft.y += GetContentOffsetTop();
	int16 contentWidth = size.x - GetContentOffsetLeft()-GetContentOffsetRight();
	int16 contentHeiht = size.y -GetContentOffsetTop()-GetContentOffsetBottom();
	int16 cellWidth = contentWidth/childItems.GetSize();
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		CIwSVec2 alignedPos = topLeft;
		alignedPos.x += (cellWidth-item->GetSize().x)*combinedStyle.HorizontalAlignment/IW_GEOM_ONE;
		alignedPos.y += (contentHeiht-item->GetSize().y)*combinedStyle.VerticalAlignment/IW_GEOM_ONE;
		item->SetOrigin(alignedPos);
		topLeft.x += cellWidth;
	}
}

//Render image on the screen surface
void CsmRow::Render(smItemContext* renderContext)
{
	CsmItem::Render(renderContext);
}


#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmRow::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
