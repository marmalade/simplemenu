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

		//Gets the element name hash to select an apropriate style
		virtual uint32 GetElementNameHash();
		virtual void ApplyStyleSheet(CsmStyleSheet* styleSheet);

		//Animate item and all child items
		virtual void Animate(iwfixed timespan);

		virtual void OnValueChanged();
		virtual void OnTemporalValueChanged();

		virtual bool KeyReleasedEvent(smKeyContext* keyContext);
		virtual bool KeyPressedEvent(smKeyContext* keyContext);

		virtual void CollectInnerTextTo(std::stringstream & s) const;
	};
}