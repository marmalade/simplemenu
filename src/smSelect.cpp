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

void CsmSelect::RearrangeChildItems()
{
	if (isOpened)
	{
		CsmItem::RearrangeChildItems();
	}
	else
	{
		if (selectedItemIndex >= 0 && selectedItemIndex < childItems.GetSize())
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
	isOpened = !isOpened;
}
void CsmSelect::TouchCanceled(smTouchContext* smTouchContext)
{
	CsmItem::TouchCanceled(smTouchContext);
}

void CsmSelect::PrepareChildItems(smItemContext* renderContext,int16 width)
{
	if (isOpened)
	{
		CsmItem::PrepareChildItems(renderContext, width);
	}
	else
	{
		int16 contentWidth = width - GetContentOffsetLeft()-GetContentOffsetRight();
		size.x = width;
		size.y = 0;
		if (selectedItemIndex >= 0 && selectedItemIndex < childItems.GetSize())
		{
			CIwManaged** i = childItems.GetBegin()+selectedItemIndex;
			CsmItem* item = static_cast<CsmItem*>(*i);
			item->Prepare(renderContext,contentWidth);
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
		if (selectedItemIndex >= 0 && selectedItemIndex < childItems.GetSize())
		{
			CIwManaged** i = childItems.GetBegin()+selectedItemIndex;
			CsmItem* item = static_cast<CsmItem*>(*i);
			item->Render(renderContext);
		}
	}
}