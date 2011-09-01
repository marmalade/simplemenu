#pragma once

#include "smImageSource.h"
#include "smTileMapSource.h"

namespace SimpleMenu
{
	class CsmTileMapImageSource: public CsmImageSource
	{
		CsmTileMapSource* source;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmTileMapImageSource);
		//Constructor
		CsmTileMapImageSource();
		//Desctructor
		virtual ~CsmTileMapImageSource();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		// Check if imge is available (loaded, downloaded etc)
		virtual bool IsAvailable() const ;
		// Get recommended size of the image
		virtual CIwSVec2 GetRecommendedSize(const CIwSVec2& area) const ;
		// Prepare image source to render the image
		virtual void Prepare(const CIwSVec2& recommendedSize);
		// Get image material to draw quad
		virtual CIwMaterial* GetMaterial();

		void SetTileMapSource(CsmTileMapSource*);

#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
		//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
		virtual	void	ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild);
#endif
	};
}