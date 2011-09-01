#pragma once

#include "smCanvas.h"

namespace SimpleMenu
{
	class CsmSelect : public CsmItem
	{
	protected:
		int32 selectedItemIndex;
		bool isOpened;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmSelect);
		//Constructor
		CsmSelect();
		//Desctructor
		virtual ~CsmSelect();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
		//Gets the element name hash to select an apropriate style
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

		virtual void OnValueChanged();
		void SetSelectedItemIndex(int i);
		//Check if element can interact with user
		virtual bool IsActive() const {return true;}
		virtual void CollectInnerTextTo(std::stringstream & s) const;
	};

}