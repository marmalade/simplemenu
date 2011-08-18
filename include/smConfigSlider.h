#pragma once

#include "smSlider.h"

namespace SimpleMenu
{
	class CsmConfigSlider: public CsmSlider
	{
		std::string configValueKey;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmConfigSlider);
		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
		virtual void OnAttachToMenu(CsmMenu*,CsmItem*);
		virtual void OnValueChanged();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}