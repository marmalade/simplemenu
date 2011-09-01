#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smTileMapImageSource.h"
#include "smScriptProvider.h"
#include "smMenu.h"
#include "simplemenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmTileMapImageSource);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmTileMapImageSource);

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmTileMapImageSource::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmTileMapImageSource> d (CsmImageSource::GetClassDescription(), "CsmTileMapImageSource",
			0);
	return &d;
}

//Constructor
CsmTileMapImageSource::CsmTileMapImageSource()
{
	source = 0;
}

//Desctructor
CsmTileMapImageSource::~CsmTileMapImageSource()
{
	if (source)
		delete source;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmTileMapImageSource::Serialise ()
{
	CsmImageSource::Serialise();
	smSerialiseManaged(source);
}

// Check if imge is available (loaded, downloaded etc)
bool CsmTileMapImageSource::IsAvailable() const 
{
	return false;
}
// Get recommended size of the image
CIwSVec2 CsmTileMapImageSource::GetRecommendedSize(const CIwSVec2& area) const 
{
	return area;
}
// Prepare image source to render the image
void CsmTileMapImageSource::Prepare(const CIwSVec2& recommendedSize)
{
}
// Get image material to draw quad
CIwMaterial* CsmTileMapImageSource::GetMaterial()
{
	return 0;
}
void CsmTileMapImageSource::SetTileMapSource(CsmTileMapSource* s)
{
	if (source) delete source;
	source = s;
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmTileMapImageSource::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	return CsmImageSource::ParseAttribute(pParser, pAttrName);
}
//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
void	CsmTileMapImageSource::ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild)
{
	CsmTileMapSource* s = dynamic_cast<CsmTileMapSource*>(pChild);
	if (s)
	{
		source = s;
		return;
	}
	CsmImageSource::ParseCloseChild(pParser, pChild);
}
#endif