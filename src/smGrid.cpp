#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smGrid.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmGrid);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmGrid);

//Constructor
CsmGrid::CsmGrid()
{

}
//Desctructor
CsmGrid::~CsmGrid()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmGrid::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmGrid> d (CsmItem::GetClassDescription(), "CsmGrid",
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmGrid::Serialise ()
{
	CsmItem::Serialise();
}
void CsmGrid::PrepareChildItems(smItemContext* context, const CIwSVec2& recommendedSize)
{
	int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = recommendedSize.x;
	size.y = 0;
	CIwSVec2 chRecSize (contentWidth,recommendedSize.y);
	for (CsmItem** i = childItems.begin(); i!=childItems.end(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(context,chRecSize);
		int32 height = item->GetSize().y;
		if (size.y < height)
		size.y = height;
	}
	size.y += GetContentOffsetTop()+GetContentOffsetBottom();
}
void CsmGrid::RearrangeChildItems()
{
	CIwSVec2 topLeft = GetOrigin();
	topLeft.x += GetContentOffsetLeft();
	topLeft.y += GetContentOffsetTop();
	for (CsmItem** i = childItems.begin(); i!=childItems.end(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->SetOrigin(topLeft);
	}
}

//Render image on the screen surface
void CsmGrid::Render(smItemContext* renderContext)
{
	CsmItem::Render(renderContext);
}
uint32 CsmGrid::GetElementNameHash()
{
	static uint32 name = IwHashString("Grid");
	return name;
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmGrid::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
