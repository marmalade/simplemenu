#include <IwTextParserITX.h>
#include <IwResManager.h>
#include "SimpleMenu.h"
#include "smConfig.h"
#include "smConfigCheckbox.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmConfigCheckbox);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmConfigCheckbox);

//Reads/writes a binary file using @a IwSerialise interface.
void CsmConfigCheckbox::Serialise ()
{
	CsmCheckbox::Serialise();
	smSerialiseString(configValueKey);
}

void CsmConfigCheckbox::OnAttachToMenu(CsmMenu* m,CsmItem* p)
{
	CsmCheckbox::OnAttachToMenu(m,p);

	if (!configValueKey.empty())
	{
		selectedItemIndex = CsmConfig::GetInteger(configValueKey.c_str());
	}
}

void CsmConfigCheckbox::OnValueChanged()
{
	CsmCheckbox::OnValueChanged();

	if (!configValueKey.empty())
	{
		CsmConfig::SetInteger(configValueKey.c_str(), selectedItemIndex);
	}
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmConfigCheckbox::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp(pAttrName, "config-value"))
	{
		smReadString(pParser, &configValueKey);
		return true;
	}

	return CsmCheckbox::ParseAttribute(pParser, pAttrName);
}

#endif
