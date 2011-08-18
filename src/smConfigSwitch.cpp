#include <IwTextParserITX.h>
#include <IwResManager.h>
#include "SimpleMenu.h"
#include "smConfig.h"
#include "smConfigSwitch.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmConfigSwitch);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmConfigSwitch);

void CsmConfigSwitch::OnAttachToMenu(CsmMenu* m,CsmItem* p)
{
	CsmSwitch::OnAttachToMenu(m,p);

	if (!configValueKey.empty())
	{
		sliderValue = temporalValue = CsmConfig::GetInteger(configValueKey.c_str());
	}
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmConfigSwitch::Serialise ()
{
	CsmSwitch::Serialise();
	smSerialiseString(configValueKey);
}

void CsmConfigSwitch::OnValueChanged()
{
	CsmSwitch::OnValueChanged();

	if (!configValueKey.empty())
	{
		CsmConfig::SetInteger(configValueKey.c_str(), sliderValue);
	}
}


#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmConfigSwitch::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp(pAttrName, "config-value"))
	{
		smReadString(pParser, &configValueKey);
		return true;
	}

	return CsmSwitch::ParseAttribute(pParser, pAttrName);
}

#endif
