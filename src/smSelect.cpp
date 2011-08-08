#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smSelect.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmSelect);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmSelect);

//Constructor
CsmSelect::CsmSelect()
{
		selectedItemIndex = 0;
		isOpened = false;
}
//Desctructor
CsmSelect::~CsmSelect()
{
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmSelect::Serialise ()
{
	CsmItem::Serialise();
}
uint32 CsmSelect::GetElementNameHash()
{
	static uint32 name = IwHashString("Select");
	return name;
}
void CsmSelect::ApplyChildStyle(smItemContext* renderContext, CsmItem*child)
{
	static uint32 sliderknob = IwHashString("option");
	uint32 s = SM_ANYSTYLE;
	if (isOpened)
	{
		if (selectedItemIndex >= 0)
		{
			CsmItem*item = static_cast<CsmItem*>(childItems[selectedItemIndex]);
			if (item == child)
				s = state;
		}
	}
	else
	{
		s = state;
	}
	if (renderContext->styleSheet != 0)
		renderContext->styleSheet->Apply(child->GetCombinedStyle(), sliderknob, styleClass, s);
}

void CsmSelect::RearrangeChildItems()
{
	if (isOpened)
	{
		CsmItem::RearrangeChildItems();
	}
	else
	{
		if (selectedItemIndex >= 0 && selectedItemIndex < (int32)childItems.GetSize())
		{
			CIwSVec2 topLeft = GetOrigin();
			topLeft.x += GetContentOffsetLeft();
			topLeft.y += GetContentOffsetTop();
			CIwManaged** i = childItems.GetBegin()+selectedItemIndex;
			CsmItem* item = static_cast<CsmItem*>(*i);
			item->SetOrigin(topLeft);
		}
	}
}
void CsmSelect::Touch(smTouchContext* smTouchContext)
{
	CsmItem::Touch(smTouchContext);
}
void CsmSelect::TouchReleased(smTouchContext* smTouchContext)
{
	CsmItem::TouchReleased(smTouchContext);
	if (isOpened)
	{
		int index = 0;
		for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
		{
			CsmItem* item = static_cast<CsmItem*>(*i);
			const CIwVec2& pos = smTouchContext->currentPoistion;
			CIwSVec2 o = item->GetOrigin();
			CIwSVec2 s = item->GetSize();
			if (pos.x >= o.x && pos.y >= o.y && pos.x < o.x+s.x && pos.y < o.y+s.y)
			{
				selectedItemIndex = index;
				break;
			}
			++index;
		}
	}
	isOpened = !isOpened;
}
void CsmSelect::TouchCanceled(smTouchContext* smTouchContext)
{
	CsmItem::TouchCanceled(smTouchContext);
}
bool CsmSelect::KeyReleasedEvent(smKeyContext* keyContext)
{
	if (isOpened)
	{
		switch (keyContext->key)
		{
		case s3eKey::s3eKeyUp:
			if (selectedItemIndex > 0)
				--selectedItemIndex;
			return true;
		case s3eKey::s3eKeyDown:
			if (selectedItemIndex < (int32)childItems.GetSize()-1)
				++selectedItemIndex;
			return true;
		default:
			break;
		}
	}
	return false;
}
bool CsmSelect::KeyPressedEvent(smKeyContext* keyContext)
{
	if (isOpened)
	{
		switch (keyContext->key)
		{
		case s3eKey::s3eKeyUp:
		case s3eKey::s3eKeyDown:
			return true;
		default:
			break;
		}
	}
	return false;
}

void CsmSelect::PrepareChildItems(smItemContext* renderContext, const CIwSVec2& recommendedSize)
{
	if (isOpened)
	{
		CsmItem::PrepareChildItems(renderContext,recommendedSize);
	}
	else
	{
		int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
		size.x = recommendedSize.x;
		size.y = 0;
		if (selectedItemIndex >= 0 && selectedItemIndex < (int32)childItems.GetSize())
		{
			CIwManaged** i = childItems.GetBegin()+selectedItemIndex;
			CsmItem* item = static_cast<CsmItem*>(*i);
			item->Prepare(renderContext,CIwSVec2(contentWidth, recommendedSize.y));
			size.y += item->GetSize().y;
		}
		size.y += GetContentOffsetTop()+GetContentOffsetBottom();
	}
}
void CsmSelect::RenderChildren(smItemContext* renderContext)
{
	if (isOpened)
	{
		CsmItem::RenderChildren(renderContext);
	}
	else
	{
		if (selectedItemIndex >= 0 && selectedItemIndex < (int32)childItems.GetSize())
		{
			CIwManaged** i = childItems.GetBegin()+selectedItemIndex;
			CsmItem* item = static_cast<CsmItem*>(*i);
			item->Render(renderContext);
		}
	}
}