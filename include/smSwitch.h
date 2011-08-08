#pragma once

#include "smSlider.h"

namespace SimpleMenu
{
	class CsmSwitch : public CsmSlider
	{
		public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmSwitch);
		//Constructor
		CsmSwitch();
		//Desctructor
		virtual ~CsmSwitch();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Animate item and all child items
		virtual void Animate(iwfixed timespan);

		virtual void Touch(smTouchContext* smTouchContext);
		virtual void TouchReleased(smTouchContext* smTouchContext);
		virtual void TouchCanceled(smTouchContext* smTouchContext);
		virtual void TouchMotion(smTouchContext* smTouchContext);
		virtual bool KeyReleasedEvent(smKeyContext* keyContext);
		virtual bool KeyPressedEvent(smKeyContext* keyContext);
	};
}