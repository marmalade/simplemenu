#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smSwitch.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmSwitch);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmSwitch);

//Constructor
CsmSwitch::CsmSwitch()
{
}
//Desctructor
CsmSwitch::~CsmSwitch()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmSwitch::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmSlider> d ("CsmSwitch",
			ScriptTraits::Method("GetRoot", &CsmSlider::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmSlider::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmSlider::GetChildItemsCount),
			0);
	return &d;
}

//Animate item and all child items
void CsmSwitch::Animate(iwfixed timespan)
{
	CsmSlider::Animate(timespan);
}

void CsmSwitch::Touch(smTouchContext* smTouchContext)
{
	CsmSlider::Touch(smTouchContext);
}
void CsmSwitch::TouchReleased(smTouchContext* smTouchContext)
{
	CsmSlider::TouchReleased(smTouchContext);
	if (sliderValue < IW_GEOM_ONE/2)
		sliderValue = 0;
	else
		sliderValue = IW_GEOM_ONE;
}
void CsmSwitch::TouchCanceled(smTouchContext* smTouchContext)
{
	CsmSlider::TouchCanceled(smTouchContext);
}
void CsmSwitch::TouchMotion(smTouchContext* smTouchContext)
{
	CsmSlider::TouchMotion(smTouchContext);
}