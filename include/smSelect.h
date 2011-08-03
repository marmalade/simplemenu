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

		virtual void RearrangeChildItems();
		virtual void PrepareChildItems(smItemContext* renderContext,int16 width);
		virtual void RenderChildren(smItemContext* renderContext);

		virtual void Touch(smTouchContext* smTouchContext);
		virtual void TouchReleased(smTouchContext* smTouchContext);
		virtual void TouchCanceled(smTouchContext* smTouchContext);
		//Check if element can interact with user
		virtual bool IsActive() const {return true;}
	};

}