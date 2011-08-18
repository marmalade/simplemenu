#include <IwTextParserITX.h>
#include <IwResManager.h>
#include "SimpleMenu.h"
#include "smConfigSlider.h"
#include "smConfig.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmConfigSlider);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmConfigSlider);

//Reads/writes a binary file using @a IwSerialise interface.
void CsmConfigSlider::Serialise ()
{
	CsmSlider::Serialise();
	smSerialiseString(configValueKey);
}

void CsmConfigSlider::OnAttachToMenu(CsmMenu* m,CsmItem* p)
{
	CsmSlider::OnAttachToMenu(m,p);

	if (!configValueKey.empty())
	{
		sliderValue = temporalValue = CsmConfig::GetInteger(configValueKey.c_str());
	}
}
void CsmConfigSlider::OnValueChanged()
{
	CsmSlider::OnValueChanged();

	if (!configValueKey.empty())
	{
		CsmConfig::SetInteger(configValueKey.c_str(), sliderValue);
	}
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmConfigSlider::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp(pAttrName, "config-value"))
	{
		smReadString(pParser, &configValueKey);
		return true;
	}

	return CsmSlider::ParseAttribute(pParser, pAttrName);
}

#endif
