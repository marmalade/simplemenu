#include <IwTextParserITX.h>
#include <IwResManager.h>
#include "SimpleMenu.h"
#include "smConfig.h"
#include "smConfigTextBox.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmConfigTextBox);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmConfigTextBox);

//Reads/writes a binary file using @a IwSerialise interface.
void CsmConfigTextBox::Serialise ()
{
	CsmTextBox::Serialise();
	smSerialiseString(configValueKey);
}

void CsmConfigTextBox::OnAttachToMenu(CsmMenu* m,CsmItem* p)
{
	CsmTextBox::OnAttachToMenu(m,p);

	if (!configValueKey.empty())
	{
		SetText(CsmConfig::GetString(configValueKey.c_str()));
	}
}
void CsmConfigTextBox::OnTextChanged()
{
	CsmTextBox::OnTextChanged();

	if (!configValueKey.empty())
	{
		CsmConfig::SetString(configValueKey.c_str(), GetText());
	}
}
#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmConfigTextBox::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp(pAttrName, "config-value"))
	{
		smReadString(pParser, &configValueKey);
		return true;
	}

	return CsmTextBox::ParseAttribute(pParser, pAttrName);
}

#endif
