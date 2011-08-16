#pragma once

#include "smSelect.h"

namespace SimpleMenu
{
	class CsmConfigSelect: public CsmSelect
	{
		std::string configValueKey;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmConfigSelect);

#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}