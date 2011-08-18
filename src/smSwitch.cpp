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
	static  TsmScriptableClassDeclaration<CsmSwitch> d (CsmSlider::GetClassDescription(), "CsmSwitch",
			0);
	return &d;
}

//Animate item and all child items
void CsmSwitch::Animate(iwfixed timespan)
{
	CsmSlider::Animate(timespan);
}
void CsmSwitch::OnValueChanged()
{
	if (sliderValue < IW_GEOM_ONE/2)
	{
		sliderValue = 0;
	}
	else
	{
		sliderValue = IW_GEOM_ONE;
	}
	CsmSlider::OnValueChanged();
}
uint32 CsmSwitch::GetElementNameHash()
{
	static uint32 name = IwHashString("Switch");
	return name;
}
void CsmSwitch::ApplyStyleSheet(CsmStyleSheet* styleSheet)
{
	static uint32 onHash = IwHashString("on");
	static uint32 offHash = IwHashString("off");
	iwfixed v = (temporalValue>=0)?temporalValue:sliderValue;
	if (styleSheet)
		styleSheet->Apply(&combinedStyle,GetElementNameHash(),GetElementClassHash(),(v<IW_GEOM_ONE/2)?offHash:onHash);
}

void CsmSwitch::OnTemporalValueChanged()
{
}

bool CsmSwitch::KeyReleasedEvent(smKeyContext* keyContext)
{
	switch (keyContext->key)
	{
	case s3eKeyLeft:
		sliderValue = 0;
		OnValueChanged();
		return true;
	case s3eKeyRight:
		sliderValue = IW_GEOM_ONE;
		OnValueChanged();
		return true;
	default:
		break;
	}
	return false;
}
bool CsmSwitch::KeyPressedEvent(smKeyContext* keyContext)
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
