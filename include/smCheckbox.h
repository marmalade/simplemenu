#pragma once

#include "smCanvas.h"

namespace SimpleMenu
{
	class CsmCheckbox : public CsmItem
	{
	protected:
		int32 selectedItemIndex;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmCheckbox);
		//Constructor
		CsmCheckbox();
		//Desctructor
		virtual ~CsmCheckbox();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
		//Gets the element name hash to Checkbox an apropriate style
		virtual uint32 GetElementNameHash();
		virtual void ApplyChildStyle(smItemContext* renderContext, CsmItem*child);

		virtual void RearrangeChildItems();
		virtual void PrepareChildItems(smItemContext* renderContext, const CIwSVec2& recommendedSize);
		virtual void RenderChildren(smItemContext* renderContext);

		virtual void Touch(smTouchContext* smTouchContext);
		virtual void TouchReleased(smTouchContext* smTouchContext);
		virtual void TouchCanceled(smTouchContext* smTouchContext);
		virtual bool KeyReleasedEvent(smKeyContext* keyContext);
		virtual bool KeyPressedEvent(smKeyContext* keyContext);
		//Check if element can interact with user
		virtual bool IsActive() const {return true;}
	};

}