#pragma once

#include <smGallery.h>

namespace SimpleMenu
{
	class CsmImageGallery : public CsmGallery
	{
	protected:
		iwfixed carouselPosition;
		bool touched;
		std::string path;
		std::string pattern;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmImageGallery);
		//Constructor
		CsmImageGallery();
		//Desctructor
		virtual ~CsmImageGallery();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
		virtual void OnAttachToMenu(CsmMenu*,CsmItem*);
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}