#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smCheckbox.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmCheckbox);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmCheckbox);

//Constructor
CsmCheckbox::CsmCheckbox()
{
		selectedItemIndex = 0;
}
//Desctructor
CsmCheckbox::~CsmCheckbox()
{
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmCheckbox::Serialise ()
{
	CsmItem::Serialise();
}
uint32 CsmCheckbox::GetElementNameHash()
{
	static uint32 name = IwHashString("Checkbox");
	return name;
}
void CsmCheckbox::ApplyChildStyle(smItemContext* renderContext, CsmItem*child)
{
	CsmItem::ApplyChildStyle(renderContext, child);
}

void CsmCheckbox::RearrangeChildItems()
{
	if (selectedItemIndex >= 0 && selectedItemIndex < (int32)childItems.size())
	{
		CIwSVec2 topLeft = GetOrigin();
		topLeft.x += GetContentOffsetLeft();
		topLeft.y += GetContentOffsetTop();
		CsmItem* item = *(childItems.begin()+selectedItemIndex);
		item->SetOrigin(topLeft);
	}
}
void CsmCheckbox::Touch(smTouchContext* smTouchContext)
{
	CsmItem::Touch(smTouchContext);
}
void CsmCheckbox::TouchReleased(smTouchContext* smTouchContext)
{
	CsmItem::TouchReleased(smTouchContext);
	SetSelectedItemIndex(selectedItemIndex+1);
}
void CsmCheckbox::OnValueChanged()
{
}
void CsmCheckbox::SetSelectedItemIndex(int i)
{
	if (i < 0)
		i = 0;
	if (i >= (int32)childItems.size())
		i = 0;
	if (selectedItemIndex != i)
	{
		selectedItemIndex = i;
		OnValueChanged();
	}
}

void CsmCheckbox::TouchCanceled(smTouchContext* smTouchContext)
{
	CsmItem::TouchCanceled(smTouchContext);
}
bool CsmCheckbox::KeyReleasedEvent(smKeyContext* keyContext)
{
	return false;
}
bool CsmCheckbox::KeyPressedEvent(smKeyContext* keyContext)
{
	return false;
}

void CsmCheckbox::PrepareChildItems(smItemContext* renderContext, const CIwSVec2& recommendedSize)
{
	int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = recommendedSize.x;
	size.y = 0;
	if (selectedItemIndex >= 0 && selectedItemIndex < (int32)childItems.size())
	{
		CsmItem* item = childItems[selectedItemIndex];
		item->Prepare(renderContext,CIwSVec2(contentWidth, recommendedSize.y));
		size.y += item->GetSize().y;
	}
	size.y += GetContentOffsetTop()+GetContentOffsetBottom();
}
void CsmCheckbox::RenderChildren(smItemContext* renderContext)
{
	if (selectedItemIndex >= 0 && selectedItemIndex < (int32)childItems.size())
	{
		childItems[selectedItemIndex]->Render(renderContext);
	}
}