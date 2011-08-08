#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smGallery.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmGallery);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmGallery);

//Constructor
CsmGallery::CsmGallery()
{
	carouselPosition = 0;
	touched = false;
}
//Desctructor
CsmGallery::~CsmGallery()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmGallery::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmGallery> d ("CsmGallery",
			ScriptTraits::Method("GetRoot", &CsmGallery::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmGallery::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmGallery::GetChildItemsCount),
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmGallery::Serialise ()
{
	CsmItem::Serialise();
	IwSerialiseInt32(carouselPosition);
}
void CsmGallery::PrepareChildItems(smItemContext* context, const CIwSVec2& recommendedSize)
{
	int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = recommendedSize.x;
	size.y = 0;
	CIwSVec2 chRecSize (contentWidth,recommendedSize.y);
	if (!touched)
	{
		iwfixed desiredPosition = carouselPosition & ~(IW_GEOM_ONE-1);
		if ((carouselPosition & (IW_GEOM_ONE-1)) > IW_GEOM_ONE/2)
			desiredPosition += IW_GEOM_ONE;
		if (desiredPosition > (GetChildItemsCount()-1)*IW_GEOM_ONE )
			desiredPosition = (GetChildItemsCount()-1)*IW_GEOM_ONE;
		if (desiredPosition < 0 )
			desiredPosition = 0;
	
		carouselPosition = carouselPosition+(desiredPosition-carouselPosition)/2;
	}
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
void CsmGallery::RearrangeChildItems()
{
	CIwSVec2 topLeft = GetOrigin();
	topLeft.x += GetContentOffsetLeft();
	topLeft.y += GetContentOffsetTop();
	int16 contentWidth = size.x - GetContentOffsetLeft()-GetContentOffsetRight();
	topLeft.x -= carouselPosition*size.x/IW_GEOM_ONE;
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->SetOrigin(topLeft);
		topLeft.x += size.x;
	}
}
void CsmGallery::TouchMotion(smTouchContext* smTouchContext)
{
	int32 shift = smTouchContext->currentPoistion.x - smTouchContext->lastKnownPoistion.x;
	int16 contentWidth = size.x - GetContentOffsetLeft()-GetContentOffsetRight();
	iwfixed v = shift*IW_GEOM_ONE/(size.x);
	carouselPosition -= v;
}
void CsmGallery::Touch(smTouchContext* smTouchContext)
{
	touched = true;
}
void CsmGallery::TouchReleased(smTouchContext* smTouchContext)
{
	touched = false;
	
}
void CsmGallery::TouchCanceled(smTouchContext* smTouchContext)
{
	touched = false;
	
}
//Render image on the screen surface
void CsmGallery::Render(smItemContext* renderContext)
{
	CsmItem::Render(renderContext);
	if (touched)
	{
		int scrollHeight = DPI::CdpiLength(IW_GEOM_ONE*5,DPI::CdpiLength::PT).GetPx(1);

		
		iwfixed max = (GetChildItemsCount())*IW_GEOM_ONE;
		iwfixed off = carouselPosition*size.x/max;
		iwfixed w = IW_GEOM_ONE*size.x/max;
		smDrawSimpleMenuScrollbar(
		CIwSVec2(origin.x-1,origin.y-1+size.y-scrollHeight),
		CIwSVec2(size.x+2,scrollHeight+2),
		CIwSVec2(origin.x+off,origin.y+size.y-scrollHeight),
		CIwSVec2(w,scrollHeight));
	}
}
uint32 CsmGallery::GetElementNameHash()
{
	static uint32 name = IwHashString("Gallery");
	return name;
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmGallery::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
