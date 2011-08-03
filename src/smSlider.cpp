#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smSlider.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmSlider);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmSlider);

//Constructor
CsmSlider::CsmSlider()
{
	sliderWidth = IW_GEOM_ONE/2;
	sliderValue = 0;
}
//Desctructor
CsmSlider::~CsmSlider()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmSlider::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmSlider> d ("CsmSlider",
			ScriptTraits::Method("GetRoot", &CsmSlider::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmSlider::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmSlider::GetChildItemsCount),
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmSlider::Serialise ()
{
	CsmItem::Serialise();
	IwSerialiseInt32(sliderWidth);
}
void CsmSlider::PrepareChildItems(smItemContext* context,int16 width)
{
	int16 contentWidth = width - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = width;
	size.y = 0;
	int16 sliderContentWidth = contentWidth*sliderWidth/IW_GEOM_ONE;

	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(context,sliderContentWidth);
		int32 height = item->GetSize().y;
		if (size.y < height)
		size.y = height;
	}
	size.y += GetContentOffsetTop()+GetContentOffsetBottom();
}
void CsmSlider::RearrangeChildItems()
{
	CIwSVec2 topLeft = GetOrigin();
	topLeft.x += GetContentOffsetLeft();
	topLeft.y += GetContentOffsetTop();
	int16 contentWidth = size.x - GetContentOffsetLeft()-GetContentOffsetRight();
	int16 sliderContentWidth = contentWidth*sliderWidth/IW_GEOM_ONE;
	int16 sliderOffset = (contentWidth-sliderContentWidth)*sliderValue/IW_GEOM_ONE;
	topLeft.x += sliderOffset;
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->SetOrigin(topLeft);
	}
}
void CsmSlider::TouchMotion(smTouchContext* smTouchContext)
{
	int32 shift = smTouchContext->currentPoistion.x - smTouchContext->lastKnownPoistion.x;
	int16 contentWidth = size.x - GetContentOffsetLeft()-GetContentOffsetRight();
	int16 sliderContentWidth = contentWidth*sliderWidth/IW_GEOM_ONE;
	iwfixed v = shift*IW_GEOM_ONE/(contentWidth-sliderContentWidth);
	sliderValue += v;
	if (sliderValue < 0 )
		sliderValue = 0;
	if (sliderValue > IW_GEOM_ONE )
		sliderValue = IW_GEOM_ONE;
}
//Render image on the screen surface
void CsmSlider::Render(smItemContext* renderContext)
{
	CsmItem::Render(renderContext);
}
uint32 CsmSlider::GetElementNameHash()
{
	static uint32 name = IwHashString("Slider");
	return name;
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmSlider::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
