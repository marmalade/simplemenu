#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "sm3DModel.h"
#include "smMenu.h"
#include "fthFont.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(Csm3DModel);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(Csm3DModel);

//Get scriptable class declaration
CsmScriptableClassDeclaration* Csm3DModel::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<Csm3DModel> d ("Csm3DModel",
			ScriptTraits::Method("GetRoot", &Csm3DModel::GetRoot),
			ScriptTraits::Method("GetChildAt", &Csm3DModel::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &Csm3DModel::GetChildItemsCount),
			0);
	return &d;
}

//Constructor
Csm3DModel::Csm3DModel()
{
	Init3DModel();
}
//Constructor
Csm3DModel::Csm3DModel(uint32 t)
{
	Init3DModel();
	textureHash = t;
}
//Desctructor
Csm3DModel::~Csm3DModel()
{
	if (material)
		delete material;
}
void Csm3DModel::Init3DModel()
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
void Csm3DModel::Serialise ()
{
	CsmTerminalItem::Serialise();
	IwSerialiseUInt32(textureHash);
}
void Csm3DModel::Prepare(smItemContext* renderContext,int16 width)
{
	if (textureHash == 0)
		return;
	CombineStyle(renderContext);
	if (!material)
	{
		texture = (CIwTexture*)IwGetResManager()->GetResHashed(textureHash, IW_GX_RESTYPE_TEXTURE);
		//CIw3DModel::Format f = texture->GetFormat();
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
void Csm3DModel::RearrangeChildItems()
{
}
//Render 3DModel on the screen surface
void Csm3DModel::Render(smItemContext* renderContext)
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
uint32 Csm3DModel::GetElementNameHash()
{
	static uint32 name = IwHashString("3DModel");
	return name;
}
#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	Csm3DModel::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
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
