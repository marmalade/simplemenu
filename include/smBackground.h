#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include <fthFont.h>
#include <dpiLength.h>

namespace SimpleMenu
{
	struct smBackgroundGradientPoint
	{
		CIwColour Colour;
		DPI::CdpiLength Position;
	};
	class CsmBackground: public CIwManaged
	{
	protected:
		uint32 textureHash;
		CIwTexture* texture;
		CIwArray<smBackgroundGradientPoint> points;
	public:
		//Constructor
		CsmBackground();
		//Desctructor
		virtual ~CsmBackground();

		virtual void Serialise();

		void Render(const CIwSVec2& origin, const CIwSVec2& size, const CIwSVec2 & viewport, const CIwMat & transformation);
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);

		virtual	void	ParseClose(CIwTextParserITX* pParser);
#endif

		bool operator == (const CsmBackground & other) const
		{
			//TODO: do a proper match
			return (textureHash == other.textureHash)
				&& (points.size() == points.size());
		}
	};
}