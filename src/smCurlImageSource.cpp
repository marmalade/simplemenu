#include "simplemenu.h"
#include "smCurlImageSource.h"
#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include <sys/param.h>

extern "C"
{
#include <jpeglib.h>
}

using namespace SimpleMenu;

namespace SimpleMenu
{

}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmCurlImageSource);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmCurlImageSource);

//Constructor
CsmCurlImageSource::CsmCurlImageSource()
{
	isActive = false;
	material = 0;
	texture = 0;
	image = 0;
	mipMapping = false;
}
//Desctructor
CsmCurlImageSource::~CsmCurlImageSource()
{
	if (material) delete material;
	if (texture) delete texture;
	if (image) delete image;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCurlImageSource::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCurlImageSource> d (0, "CsmCurlImageSource",
			//ScriptTraits::Method("GetRoot", &CsmCurlImageSource::GetRoot),
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmCurlImageSource::Serialise ()
{
	smSerialiseString(url);
}

// Check if imge is available (loaded, downloaded etc)
bool CsmCurlImageSource::IsAvailable() const
{
	return material != 0;
}
// Get recommended size of the image
CIwSVec2 CsmCurlImageSource::GetRecommendedSize(const CIwSVec2& area) const
{
	if (texture)
		return CIwSVec2((int16)texture->GetWidth(),(int16)texture->GetHeight());
	return area;
}
// Prepare image source to render the image
void CsmCurlImageSource::Prepare(const CIwSVec2& recommendedSize)
{
}
// Get image material to draw quad
CIwMaterial* CsmCurlImageSource::GetMaterial()
{
	return material;
}
void CsmCurlImageSource::SetImageToMaterial()
{
	if (!material) material = new CIwMaterial();
	if (!texture)
	{
		texture = new CIwTexture();
		texture->SetMipMapping(mipMapping);
	}
	material->SetColAmbient(255,255,255,255);
	if (image && image->GetWidth() > 0)
	{
		texture->SetImage(image);
		texture->Upload();
		material->SetTexture(texture);
	}
}

//Animate item and all child items
void CsmCurlImageSource::Animate(iwfixed timespan)
{
	if (!material && !isActive && !url.empty())
	{
		if ((url.length() > 5) && (url[0] == 'r') &&
			((url[1] == 'a') || (url[1] == 'o')) && (url[2] == 'm') && (url[3] == ':') && (url[4] == '/'))
		{
			if (image) delete image; image = new CIwImage();
			smLoadImage(url.c_str(), image);
			SetImageToMaterial();
		}
		else
		{
			isActive = true;
			request.SetUrl(url.c_str());
		}
	}
	if (isActive)
	{
		if (!request.PerformStep())
		{
			isActive = false;

			if (request.GetInputBufferSize() != 0)
			{
				const char* t = request.GetContentType();
				if (image) delete image; image = new CIwImage();
				if (!stricmp(t, "image/jpg") || !stricmp(t, "image/jpeg"))
				{
					smDecodeJpeg((void*)request.GetInputBuffer(), request.GetInputBufferSize(), image);
				}
				else
				{
					smDecodePng((void*)request.GetInputBuffer(), request.GetInputBufferSize(), image);
				}
			}
			SetImageToMaterial();
		}
	}
}

#ifdef IW_BUILD_RESOURCES
//Parses from text file: parses attribute/value pair.
bool	CsmCurlImageSource::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("url",pAttrName) || !stricmp("get",pAttrName))
	{
		smReadString(pParser, &url);
		return true;
	}
	return CsmImageSource::ParseAttribute(pParser, pAttrName);
}
#endif