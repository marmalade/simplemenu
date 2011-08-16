#pragma once

#include "smCheckbox.h"

namespace SimpleMenu
{
	class CsmConfigCheckbox: public CsmCheckbox
	{
		std::string configValueKey;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmConfigCheckbox);

#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}