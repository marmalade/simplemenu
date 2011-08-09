#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smImageSource.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmImageTexture);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmImageTexture);

//Constructor
CsmImageTexture::CsmImageTexture()
{
	InitImage();
}
//Constructor
CsmImageTexture::CsmImageTexture(uint32 t)
{
	InitImage();
	textureHash = t;
}
//Desctructor
CsmImageTexture::~CsmImageTexture()
{
	if (material)
		delete material;
}
void CsmImageTexture::InitImage()
{
	textureHash = 0;
	texture = 0;
	material = 0;
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmImageTexture::Serialise ()
{
	CsmImageSource::Serialise();
	IwSerialiseUInt32(textureHash);
}
bool CsmImageTexture::IsAvailable() const
{
	return textureHash != 0;
}
CIwSVec2 CsmImageTexture::GetRecommendedSize(const CIwSVec2& area) const
{
	if (textureHash == 0)
		return CIwSVec2(0,0);
	if (!texture)
		(const_cast<CsmImageTexture*>(this))->texture = (CIwTexture*)IwGetResManager()->GetResHashed(textureHash, "CIwTexture", IW_RES_PERMIT_NULL_F);
	if (!texture)
		return CIwSVec2(0,0);
	return CIwSVec2(texture->GetWidth(), texture->GetHeight());
}
void CsmImageTexture::Prepare(const CIwSVec2& recommendedSize)
{
}
CIwMaterial* CsmImageTexture::GetMaterial()
{
	if (!texture)
		return 0;
	if (!material)
	{
		material = new CIwMaterial();
		material->SetTexture(texture);
		material->SetColAmbient(255,255,255,255);
	}
	return material;
}
	//if (!material)
	//{
	//	texture = (CIwTexture*)IwGetResManager()->GetResHashed(textureHash, IW_GX_RESTYPE_TEXTURE);
	//	//CIwImage::Format f = texture->GetFormat();
	//	material = new CIwMaterial();
	//	material->SetTexture(texture);
	//}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmImageTexture::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("texture",pAttrName))
	{
		pParser->ReadStringHash(&textureHash);
		return true;
	}
	return CsmImageSource::ParseAttribute(pParser, pAttrName);
}

#endif
