#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "simplemenu.h"
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

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmImageSource::GetClassDescription()
{
	static  TsmScriptableAbstractClassDeclaration<CsmImageSource> d (0, "CsmImageSource",
		ScriptTraits::Method("IsAvailable", &CsmImageSource::IsAvailable),
		0);
	return &d;
}


//Constructor
CsmImageTexture::CsmImageTexture()
{
	InitImage();
}
//Constructor
CsmImageTexture::CsmImageTexture(const char* t)
{
	InitImage();
	textureName = t;
}
//Desctructor
CsmImageTexture::~CsmImageTexture()
{
	if (material)
		delete material;
	if (aggregatedTexture && texture)
		delete texture;
}
void CsmImageTexture::InitImage()
{
	texture = 0;
	material = 0;
	aggregatedTexture = false;
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmImageTexture::Serialise ()
{
	CsmImageSource::Serialise();
	smSerialiseString(textureName);
}
bool CsmImageTexture::IsAvailable() const
{
	return !textureName.empty();
}
void CsmImageTexture::LoadImage()
{
	if (textureName.length()>5 && textureName[0]=='r' && textureName[1]=='o' && textureName[2]=='m' && textureName[3]==':')
	{
		CIwImage image;
		smLoadImage(textureName.c_str(), &image);
		aggregatedTexture = true;
		texture = new CIwTexture();
		if (image.GetWidth() > 0)
		{
			texture->SetImage(&image);
			texture->Upload();
		}
	}
	else
	{
		aggregatedTexture = false;
		texture = (CIwTexture*)IwGetResManager()->GetResHashed(IwHashString(textureName.c_str()), "CIwTexture", IW_RES_PERMIT_NULL_F);
	}
}
CIwSVec2 CsmImageTexture::GetRecommendedSize(const CIwSVec2& area) const
{
	if (!IsAvailable())
		return CIwSVec2(0,0);
	if (!texture)
	{
		(const_cast<CsmImageTexture*>(this))->LoadImage();
	}
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
		if (texture->GetWidth() > 0)
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
		smReadString(pParser, &textureName);
		return true;
	}
	return CsmImageSource::ParseAttribute(pParser, pAttrName);
}

#endif
