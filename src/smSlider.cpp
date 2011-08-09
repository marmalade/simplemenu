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
void CsmSlider::PrepareChildItems(smItemContext* context, const CIwSVec2& recommendedSize)
{
	int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = recommendedSize.x;
	size.y = 0;
	int16 sliderContentWidth = contentWidth*sliderWidth/IW_GEOM_ONE;
	CIwSVec2 chRecSize (sliderContentWidth,recommendedSize.y);

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
void CsmSlider::ApplyChildStyle(smItemContext* renderContext, CsmItem*child)
{
	static uint32 sliderknob = IwHashString("Sliderknob");
	if (renderContext->styleSheet != 0)
		renderContext->styleSheet->Apply(child->GetCombinedStyle(), sliderknob, styleClass, state);
}
bool CsmSlider::KeyReleasedEvent(smKeyContext* keyContext)
{
	switch (keyContext->key)
	{
	case s3eKeyLeft:
		sliderValue -= IW_GEOM_ONE/10;
		if (sliderValue < 0)
			sliderValue = 0;
		return true;
	case s3eKeyRight:
		sliderValue += IW_GEOM_ONE/10;
		if (sliderValue > IW_GEOM_ONE)
			sliderValue = IW_GEOM_ONE;
		return true;
	default:
		break;
	}
	return false;
}
bool CsmSlider::KeyPressedEvent(smKeyContext* keyContext)
{
	switch (keyContext->key)
	{
	case s3eKeyLeft:
	case s3eKeyRight:
		return true;
	default:
		break;
	}
	return false;
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmSlider::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmItem::ParseAttribute(pParser, pAttrName);
}

#endif
