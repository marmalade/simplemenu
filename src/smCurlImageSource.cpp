#include "simplemenu.h"
#include "smCurlImageSource.h"

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
	static  TsmScriptableClassDeclaration<CsmCurlImageSource> d ("CsmCurlImageSource",
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
//Animate item and all child items
void CsmCurlImageSource::Animate(iwfixed timespan)
{
	if (!material && !isActive && !url.empty())
	{
		isActive = true;
		request.SetUrl(url.c_str());
	}
	if (isActive)
	{
		if (!request.PerformStep())
		{
			isActive = false;
			material = new CIwMaterial();
			texture = new CIwTexture();
			material->SetColAmbient(255,255,255,255);

			//const char* t = request.GetContentType();

			//if (!stricmp(t, "image/jpg") || !stricmp(t, "image/jpeg"))
			//{
			//}
			//else if (!stricmp(t, "image/png"))
			//{
			//}

			image = new CIwImage();
			if (request.GetInputBufferSize() > 0)
			{
				s3eFile* f =  s3eFileOpenFromMemory((void*)request.GetInputBuffer(), request.GetInputBufferSize());
				if (f)
				{
					image->ReadFile(f);
					s3eFileClose(f);
				}
			}
			texture->SetImage(image);
			texture->Upload();
			material->SetTexture(texture);
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