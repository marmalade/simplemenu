#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smImage.h"
#include "smMenu.h"
#include "fthFont.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmImage);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmImage);

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmImage::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmImage> d ("CsmImage",
			ScriptTraits::Method("GetRoot", &CsmImage::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmImage::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmImage::GetChildItemsCount),
			0);
	return &d;
}

//Constructor
CsmImage::CsmImage()
{
	InitImage();
}
//Constructor
CsmImage::CsmImage(uint32 t)
{
	InitImage();
	textureHash = t;
}
//Desctructor
CsmImage::~CsmImage()
{
	if (material)
		delete material;
}
void CsmImage::InitImage()
{
	textureHash = 0;
	texture = 0;
	material = 0;
	rectPos = CIwSVec2::g_Zero;
	rectColour.Set(0xFFFFFFFF);

	styleSheetHash = 0;
	styleSheet = 0;

}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmImage::Serialise ()
{
	CsmTerminalItem::Serialise();
	IwSerialiseUInt32(textureHash);
}
void CsmImage::Prepare(smItemContext* renderContext,int16 width)
{
	if (textureHash == 0)
		return;
	CombineStyle(renderContext);
	if (!material)
	{
		texture = (CIwTexture*)IwGetResManager()->GetResHashed(textureHash, IW_GX_RESTYPE_TEXTURE);
		//CIwImage::Format f = texture->GetFormat();
		material = new CIwMaterial();
		material->SetTexture(texture);
	}
	rectSize = CIwSVec2::g_Zero;
	if (texture)
	{
		rectSize.x = texture->GetWidth();
		rectSize.y = texture->GetHeight();
		int16 contentWidth = width-(GetMarginLeft()+GetPaddingLeft()+GetMarginRight()+GetPaddingRight());
		if (rectSize.x > contentWidth)
		{
			rectSize.y = (int16)((int32)rectSize.y*(int32)width/(int32)rectSize.x);
			rectSize.x = contentWidth;
		}
		size.y = rectSize.y;
		size.x = contentWidth;
	}
	RearrangeChildItems();
}
void CsmImage::RearrangeChildItems()
{
}
//Render image on the screen surface
void CsmImage::Render(smItemContext* renderContext)
{
	if (textureHash == 0)
		return;

	CIwSVec2 rectPos = GetOrigin();
	rectPos.y += GetMarginTop()+GetPaddingTop();
	int16 width = GetSize().x;
	rectPos.x += GetMarginLeft()+GetPaddingLeft();
	int16 contentWidth = width-(GetMarginLeft()+GetPaddingLeft()+GetMarginRight()+GetPaddingRight());
	rectPos.x += (contentWidth-rectSize.x)*combinedStyle.HorizontalAlignment/IW_GEOM_ONE;

	IwGxSetMaterial(material);
	CIwSVec2* v = IW_GX_ALLOC(CIwSVec2,4);
	v[0] = CIwSVec2(rectPos.x, rectPos.y);
	v[1] = CIwSVec2(rectPos.x, rectPos.y+rectSize.y);
	v[2] = CIwSVec2(rectPos.x+rectSize.x, rectPos.y+rectSize.y);
	v[3] = CIwSVec2(rectPos.x+rectSize.x, rectPos.y);
	CIwSVec2* uv = IW_GX_ALLOC(CIwSVec2,4);
	uv[0] = CIwSVec2(0,0);
	uv[1] = CIwSVec2(0,IW_GEOM_ONE);
	uv[2] = CIwSVec2(IW_GEOM_ONE,IW_GEOM_ONE);
	uv[3] = CIwSVec2(IW_GEOM_ONE,0);
	CIwColour* col =IW_GX_ALLOC(CIwColour,4);
	col[0] = col[1] = col[2] = col[3] = rectColour;

	//smTransformScreenSpace3D(v,v+4,renderContext->transformation, renderContext->viewportSize);

	IwGxSetVertStreamScreenSpace(v,4);
	IwGxSetColStream(col);
	IwGxSetUVStream(uv);
	IwGxDrawPrims(IW_GX_QUAD_LIST, 0, 4);
}
uint32 CsmImage::GetElementNameHash()
{
	static uint32 name = IwHashString("IMAGE");
	return name;
}
#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmImage::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("texture",pAttrName))
	{
		pParser->ReadStringHash(&textureHash);
		return true;
	}
	if (!stricmp("styleSheet",pAttrName))
	{
		pParser->ReadStringHash(&styleSheetHash);
		return true;
	}
	return CsmTerminalItem::ParseAttribute(pParser, pAttrName);
}

#endif
