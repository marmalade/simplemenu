#pragma once

#include "smTextBox.h"

namespace SimpleMenu
{
	class CsmConfigTextBox: public CsmTextBox
	{
		std::string configValueKey;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmConfigTextBox);
		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
		virtual void OnAttachToMenu(CsmMenu*,CsmItem*);
		virtual void OnTextChanged();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}