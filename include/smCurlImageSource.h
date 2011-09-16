#pragma once

#include <smImageSource.h>
#include "smCurlRequest.h"

namespace SimpleMenu
{
	class CsmCurlImageSource: public CsmImageSource
	{
	protected:
		std::string url;
		CsmCurlMemoryRequest request;
		bool isActive;
		CIwMaterial* material;
		CIwTexture* texture;
		CIwImage* image;
		bool mipMapping;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmCurlImageSource);
		//Constructor
		CsmCurlImageSource();
		//Desctructor
		virtual ~CsmCurlImageSource();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		//Animate item and all child items
		virtual void Animate(iwfixed timespan);

		// Check if imge is available (loaded, downloaded etc)
		virtual bool IsAvailable() const;
		// Get recommended size of the image
		virtual CIwSVec2 GetRecommendedSize(const CIwSVec2& area) const;
		// Prepare image source to render the image
		virtual void Prepare(const CIwSVec2& recommendedSize);
		// Get image material to draw quad
		virtual CIwMaterial* GetMaterial();

		void SetImageToMaterial();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}