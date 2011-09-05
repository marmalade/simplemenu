#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smImage.h"
#include "smImageSource.h"
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
	static  TsmScriptableClassDeclaration<CsmImage> d (CsmTerminalItem::GetClassDescription(), "CsmImage",
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
	source = new CsmImageTexture(t);
}
//Desctructor
CsmImage::~CsmImage()
{
	if (source)
		delete source;
}
void CsmImage::InitImage()
{
	source = 0;
	rectPos = CIwSVec2::g_Zero;
	rectColour.Set(0xFFFFFFFF);

	styleSheetHash = 0;
	styleSheet = 0;

}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmImage::Serialise ()
{
	CsmTerminalItem::Serialise();
	CIwManaged* m = source;
	IwSerialiseManagedObject(m);
	if (m) source = static_cast<CsmImageSource*>(m);
}
void CsmImage::Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize)
{
	CombineStyle(renderContext);

	if (source == 0)
		return;
	if (!source->IsAvailable())
		return;

	int16 contentWidth = recommendedSize.x - GetContentOffsetLeft()-GetContentOffsetRight();
	int16 contentHeight = recommendedSize.y - GetContentOffsetTop()-GetContentOffsetBottom();

	rectSize = source->GetRecommendedSize(CIwSVec2(contentWidth,contentHeight));

	if (rectSize.x > contentWidth)
	{
		rectSize.y = (int16)((int32)rectSize.y*(int32)contentWidth/(int32)rectSize.x);
		rectSize.x = contentWidth;
	}
	if (rectSize.y > contentHeight)
	{
		rectSize.x = (int16)((int32)rectSize.x*(int32)contentHeight/(int32)rectSize.y);
		rectSize.y = contentHeight;
	}
	size.y = rectSize.y+ GetContentOffsetTop()+GetContentOffsetBottom();
	size.x = recommendedSize.x;
	source->Prepare(rectSize);
	RearrangeChildItems();
}
void CsmImage::RearrangeChildItems()
{
}
//Render image on the screen surface
void CsmImage::Render(smItemContext* renderContext)
{
	if (source == 0)
		return;
	if (!source->IsAvailable())
		return;
	CIwMaterial* m = source->GetMaterial();
	if (!m)
		return;
	CIwSVec2 rectPos = GetOrigin();
	rectPos.y += GetMarginTop()+GetPaddingTop();
	int16 width = GetSize().x;
	rectPos.x += GetMarginLeft()+GetPaddingLeft();
	int16 contentWidth = width-(GetMarginLeft()+GetPaddingLeft()+GetMarginRight()+GetPaddingRight());
	rectPos.x += (contentWidth-rectSize.x)*combinedStyle.HorizontalAlignment/IW_GEOM_ONE;

	IwGxSetMaterial(m);
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

	if (renderContext->transformation != CIwMat2D::g_Identity) for (CIwSVec2*i=v;i!=v+4;++i) *i = renderContext->transformation.TransformVec(*i);
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
//Animate item and all child items
void CsmImage::Animate(iwfixed timespan)
{
	if (source)
		source->Animate(timespan);
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmImage::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("styleSheet",pAttrName))
	{
		pParser->ReadStringHash(&styleSheetHash);
		return true;
	}
	return CsmTerminalItem::ParseAttribute(pParser, pAttrName);
}
//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
void	CsmImage::ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild)
{
	CsmImageSource* s = dynamic_cast<CsmImageSource*>(pChild);
	if (s)
	{
		source = s;
		return;
	}
	CsmTerminalItem::ParseCloseChild(pParser, pChild);
}
#endif
