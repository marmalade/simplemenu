#include <IwTextParserITX.h>
#include <IwResManager.h>
#include "SimpleMenu.h"
#include "smConfigSelect.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmConfigSelect);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmConfigSelect);

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmConfigSelect::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp(pAttrName, "config-value"))
	{
		smReadString(pParser, &configValueKey);
		return true;
	}

	return CsmSelect::ParseAttribute(pParser, pAttrName);
}

#endif
