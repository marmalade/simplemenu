#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smSmartGrid.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmSmartGrid);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmSmartGrid);

//Constructor
CsmSmartGrid::CsmSmartGrid()
{
	cellHeight = cellWidth = DPI::CdpiLength(IW_GEOM_ONE, DPI::CdpiLength::PERCENT);
}
//Desctructor
CsmSmartGrid::~CsmSmartGrid()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmSmartGrid::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmSmartGrid> d (CsmItem::GetClassDescription(), "CsmSmartGrid",
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmSmartGrid::Serialise ()
{
	CsmItem::Serialise();
	cellWidth.Serialise();
	cellHeight.Serialise();
}
void CsmSmartGrid::PrepareChildItems(smItemContext* context, const CIwSVec2& recommendedSize)
{
	size.x = recommendedSize.x;
	int16 contentWidth = size.x - GetContentOffsetLeft()-GetContentOffsetRight();
	size.y = 0;
	int16 actualCellWidth = cellWidth.GetPx(contentWidth);
	if (actualCellWidth > contentWidth) actualCellWidth = contentWidth;
	CIwSVec2 chRecSize (actualCellWidth, recommendedSize.y);
	for (CsmItem** i = childItems.begin(); i!=childItems.end(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(context,chRecSize);
	}
	
	for (CsmItem** i = childItems.begin(); i!=childItems.end();)
	{
		CsmItem** j = i;
		int16 w = 0,h=actualCellWidth;
		for (;j!=childItems.end();++j)
		{
			const CIwSVec2& childSize = (*j)->GetSize();
			if (w+childSize.x > contentWidth && i!=j) break;
			w += childSize.x;
			if (h<childSize.y) h=childSize.y;
		}
		for (;j!=i;++i)
		{
			if ((*i)->GetSize().y < h) { (*i)->SetSize(CIwSVec2(actualCellWidth,h)); (*i)->RearrangeChildItems(); }
		}
		size.y += h;
	}
	size.y += GetContentOffsetTop()+GetContentOffsetBottom();
}
void CsmSmartGrid::RearrangeChildItems()
{
	int16 contentWidth = size.x - GetContentOffsetLeft()-GetContentOffsetRight();
	int16 actualCellWidth = cellWidth.GetPx(contentWidth);
	if (actualCellWidth > contentWidth) actualCellWidth = contentWidth;
	
	CIwSVec2 topLeft = GetOrigin();
	topLeft.x += GetContentOffsetLeft();
	topLeft.y += GetContentOffsetTop();
	if (actualCellWidth > 0)
		topLeft.x += (contentWidth%actualCellWidth)*combinedStyle.HorizontalAlignment/IW_GEOM_ONE;
	int16 lineStartsAt = topLeft.x;
	for (CsmItem** i = childItems.begin(); i!=childItems.end();)
	{
		CsmItem** j = i;
		int16 w = 0,h=0;
		for (;j!=childItems.end();++j)
		{
			const CIwSVec2& childSize = (*j)->GetSize();
			if (w+childSize.x > contentWidth && i!=j) break;
			(*j)->SetOrigin(topLeft);
			topLeft.x += childSize.x;
			w += childSize.x;
			if (h<childSize.y) h=childSize.y;
		}
		i = j;
		topLeft.y += h;
		topLeft.x = lineStartsAt;
	}

}

//Render image on the screen surface
void CsmSmartGrid::Render(smItemContext* renderContext)
{
	CsmItem::Render(renderContext);
}
uint32 CsmSmartGrid::GetElementNameHash()
{
	static uint32 name = IwHashString("SmartGrid");
	return name;
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmSmartGrid::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("cell-width",pAttrName))
	{
		cellWidth.ParseAttribute(pParser);
		return true;
	}
	if (!stricmp("cell-height",pAttrName))
	{
		cellHeight.ParseAttribute(pParser);
		return true;
	}
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
