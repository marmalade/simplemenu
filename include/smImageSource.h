#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "smImage.h"

namespace SimpleMenu
{
	class CsmImageSource : public CIwManaged
	{
	public:
		// Check if imge is available (loaded, downloaded etc)
		virtual bool IsAvailable() const = 0;
		// Get recommended size of the image
		virtual CIwSVec2 GetRecommendedSize(const CIwSVec2& area) const = 0;
		// Prepare image source to render the image
		virtual void Prepare(const CIwSVec2& recommendedSize) = 0;
		// Get image material to draw quad
		virtual CIwMaterial* GetMaterial() = 0;
		//Animate item and all child items
		virtual void Animate(iwfixed timespan) {};
	};
	class CsmImageTexture : public CsmImageSource
	{
	protected:
		uint32 textureHash;
		CIwTexture* texture;
		CIwMaterial* material;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmImageTexture);
		//Constructor
		CsmImageTexture();
		//Constructor
		CsmImageTexture(uint32 hash);
		//Desctructor
		virtual ~CsmImageTexture();
		//Reads/writes a binary file using @a IwSerialise interface.
		void Serialise ();

		virtual bool IsAvailable() const;
		virtual CIwSVec2 GetRecommendedSize(const CIwSVec2& area) const;
		virtual void Prepare(const CIwSVec2& recommendedSize);
		virtual CIwMaterial* GetMaterial();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);

#endif
	protected:
		void InitImage();
	};
}