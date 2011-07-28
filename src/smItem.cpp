#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "simplemenu.h"
#include "smMenu.h"
#include "smItem.h"
#include "smImage.h"
#include "smTextBlock.h"
#include "fthFont.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmItem);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmItem);


//Constructor
CsmItem::CsmItem()
{
	origin = CIwSVec2::g_Zero;
	size = CIwSVec2::g_Zero;
	styleClass = SM_ANYSTYLE;
	state = SM_ANYSTYLE;
	idHash = 0;
}
//Desctructor
CsmItem::~CsmItem()
{
	childItems.Delete();
	while (lazyEvents.GetFirstChild())
		delete lazyEvents.GetFirstChild();
}
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmItem::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmItem> d ("CsmItem",
			ScriptTraits::Method("GetRoot", &CsmItem::GetRoot),
			ScriptTraits::Method("GetChildAt", &CsmItem::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &CsmItem::GetChildItemsCount),
			
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmItem::Serialise ()
{
	CIwManaged::Serialise();
	childItems.Serialise();
	origin.Serialise();
	size.Serialise();
	style.Serialise();
	IwSerialiseUInt32(styleClass);
	IwSerialiseUInt32(state);
	IwSerialiseUInt32(idHash);
	smSerialiseString(onUpdate);
}

void CsmItem::EvalUpdate()
{
	if (onUpdate.size() > 0)
		GetRoot()->Eval(this, onUpdate.c_str());
}
void CsmItem::Prepare(smItemContext* renderContext,int16 width)
{
	EvalUpdate();

	CombineStyle(renderContext);
	smItemContext context = *renderContext;
	context.parentStyle = &combinedStyle;

	int16 contentWidth = width - GetContentOffsetLeft()-GetContentOffsetRight();
	size.x = width;
	size.y = 0;
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(&context,contentWidth);
		size.y += item->GetSize().y;
	}
	size.y += GetContentOffsetTop()+GetContentOffsetBottom();
	RearrangeChildItems();
}
void CsmItem::RearrangeChildItems()
{
	CIwSVec2 topLeft = GetOrigin();
	topLeft.x += GetContentOffsetLeft();
	topLeft.y += GetContentOffsetTop();
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->SetOrigin(topLeft);
		topLeft.y += item->GetSize().y;
	}
}
void CsmItem::InitTree(CsmMenu*r,CsmItem*p)
{
	root = r;
	parent = p;
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->InitTree(r,this);
	}
}
bool CsmItem::IsVisible(smItemContext* renderContext)
{
	if (origin.x >= renderContext->viewportPos.x+renderContext->viewportSize.x)
		return false;
	if (origin.y >= renderContext->viewportPos.y+renderContext->viewportSize.y)
		return false;
	if (origin.x+size.x <= renderContext->viewportPos.x)
		return false;
	if (origin.y+size.y <= renderContext->viewportPos.y)
		return false;
	return true;
}
void CsmItem::RenderBackgroud(smItemContext* renderContext)
{
	combinedStyle.Background.Render(
		GetOrigin()+CIwSVec2(GetMarginLeft()+GetBorderLeft(),GetMarginTop()+GetBorderTop()), 
		GetSize()-CIwSVec2(GetMarginLeft()+GetMarginRight()+GetBorderLeft()+GetBorderRight(),GetMarginTop()+GetMarginBottom()+GetBorderTop()+GetBorderBottom()),
		renderContext->viewportSize,
		renderContext->transformation);
}
void CsmItem::RenderShadow(smItemContext* renderContext)
{
	if (!combinedStyle.DropShadow)
		return;
	if (combinedStyle.ShadowColor.a == 0)
		return;
	if (combinedStyle.ShadowSize.IsZero() && combinedStyle.ShadowOffset.IsZero())
		return;

	uint32 vertices = 16;
	uint32 numIndices = 18*3;
	CIwSVec2* v = IW_GX_ALLOC(CIwSVec2,vertices);
	CIwColour* col =IW_GX_ALLOC(CIwColour,vertices);
	uint16* indices = IW_GX_ALLOC(uint16,numIndices);
	CIwMaterial* m = IW_GX_ALLOC_MATERIAL();
	m->SetColAmbient(255,255,255,255);

	CIwSVec2 shadowSize;
	shadowSize.x=shadowSize.y= combinedStyle.ShadowSize.GetPx(1);
	CIwSVec2 leftTop = GetOrigin() + CIwSVec2(combinedStyle.ShadowOffset.x.GetPx(1) + GetMarginLeft(),combinedStyle.ShadowOffset.x.GetPx(1) + GetMarginTop());
	CIwSVec2 rightBottom = leftTop + GetSize() - CIwSVec2(GetMarginLeft()+GetMarginRight(), GetMarginTop()+GetMarginBottom());
	CIwSVec2 llTT = leftTop-shadowSize;
	CIwSVec2 rrBB = rightBottom+shadowSize;
	uint32 i = 0;
	v[i++] = CIwSVec2(leftTop.x,leftTop.y);
	v[i++] = CIwSVec2(rightBottom.x,leftTop.y);
	v[i++] = CIwSVec2(rightBottom.x,rightBottom.y);
	v[i++] = CIwSVec2(leftTop.x,rightBottom.y);

	v[i++] = CIwSVec2(llTT.x,leftTop.y);
	v[i++] = CIwSVec2((llTT.x*2+leftTop.x)/3,(llTT.y*2+leftTop.y)/3);
	v[i++] = CIwSVec2(leftTop.x,llTT.y);

	v[i++] = CIwSVec2(rightBottom.x,llTT.y);
	v[i++] = CIwSVec2((rrBB.x*2+rightBottom.x)/3,(llTT.y*2+leftTop.y)/3);
	v[i++] = CIwSVec2(rrBB.x,leftTop.y);

	v[i++] = CIwSVec2(rrBB.x,rightBottom.y);
	v[i++] = CIwSVec2((rrBB.x*2+rightBottom.x)/3,(rrBB.y*2+rightBottom.y)/3);
	v[i++] = CIwSVec2(rightBottom.x,rrBB.y);

	v[i++] = CIwSVec2(leftTop.x,rrBB.y);
	v[i++] = CIwSVec2((llTT.x*2+leftTop.x)/3,(rrBB.y*2+rightBottom.y)/3);
	v[i++] = CIwSVec2(llTT.x,rightBottom.y);

	col[0] = col[1] = col[2] = col[3] = combinedStyle.ShadowColor;
	CIwColour transparent;
	transparent.Set(0);
	col[4] = col[5] = col[6] = col[7] = transparent;
	col[8] = col[9] = col[10] = col[11] = transparent;
	col[12] = col[13] = col[14] = col[15] = transparent;
	
	i = 0;
	indices[i++] = 0;	indices[i++] = 3;	indices[i++] = 2;
	indices[i++] = 0;	indices[i++] = 2;	indices[i++] = 1;

	indices[i++] = 4;	indices[i++] = 0;	indices[i++] = 5;
	indices[i++] = 5;	indices[i++] = 0;	indices[i++] = 6;

	indices[i++] = 6;	indices[i++] = 0;	indices[i++] = 7;
	indices[i++] = 7;	indices[i++] = 0;	indices[i++] = 1;

	indices[i++] = 7;	indices[i++] = 1;	indices[i++] = 8;
	indices[i++] = 8;	indices[i++] = 1;	indices[i++] = 9;

	indices[i++] = 9;	indices[i++] = 1;	indices[i++] = 10;
	indices[i++] = 10;	indices[i++] = 1;	indices[i++] = 2;

	indices[i++] = 10;	indices[i++] = 2;	indices[i++] = 11;
	indices[i++] = 11;	indices[i++] = 2;	indices[i++] = 12;

	indices[i++] = 12;	indices[i++] = 2;	indices[i++] = 13;
	indices[i++] = 13;	indices[i++] = 2;	indices[i++] = 3;

	indices[i++] = 13;	indices[i++] = 3;	indices[i++] = 14;
	indices[i++] = 14;	indices[i++] = 3;	indices[i++] = 15;

	indices[i++] = 15;	indices[i++] = 3;	indices[i++] = 4;
	indices[i++] = 4;	indices[i++] = 3;	indices[i++] = 0;

	m->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(m);

	//toeTransformScreenSpace3D(v,v+vertices,renderContext->transformation, renderContext->viewportSize);
	IwGxSetVertStreamScreenSpace(v,vertices);
	IwGxSetColStream(col);
	IwGxSetUVStream(0);
	IwGxDrawPrims(IW_GX_TRI_LIST, indices, i);

}
void CsmItem::RenderBorder(smItemContext* renderContext)
{
	if (combinedStyle.BorderColor.a == 0)
		return;
	if (combinedStyle.Border.IsZero())
		return;

	uint32 vertices = 8;
	uint32 numIndices = 8*3;
	CIwSVec2* v = IW_GX_ALLOC(CIwSVec2,vertices);
	CIwColour* col =IW_GX_ALLOC(CIwColour,vertices);
	uint16* indices = IW_GX_ALLOC(uint16,numIndices);
	CIwMaterial* m = IW_GX_ALLOC_MATERIAL();
	m->SetColAmbient(255,255,255,255);

	CIwSVec2 leftTop = GetOrigin()+ CIwSVec2(GetMarginLeft(), GetMarginTop());
	CIwSVec2 rightBottom = leftTop + GetSize()- CIwSVec2(GetMarginLeft()+GetMarginRight(), GetMarginTop()+GetMarginBottom());
	CIwSVec2 llTT = leftTop+CIwSVec2(GetBorderLeft(),GetBorderTop());
	CIwSVec2 rrBB = rightBottom-CIwSVec2(GetBorderRight(),GetBorderBottom());

	uint32 i = 0;
	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(leftTop.x,leftTop.y);
	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(rightBottom.x,leftTop.y);
	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(rightBottom.x,rightBottom.y);
	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(leftTop.x,rightBottom.y);

	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(llTT.x,llTT.y);
	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(rrBB.x,llTT.y);
	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(rrBB.x,rrBB.y);
	col[i] = combinedStyle.BorderColor;
	v[i++] = CIwSVec2(llTT.x,rrBB.y);

	i = 0;
	indices[i++] = 0;	indices[i++] = 4;	indices[i++] = 1;
	indices[i++] = 1;	indices[i++] = 4;	indices[i++] = 5;

	indices[i++] = 1;	indices[i++] = 5;	indices[i++] = 2;
	indices[i++] = 2;	indices[i++] = 5;	indices[i++] = 6;

	indices[i++] = 2;	indices[i++] = 6;	indices[i++] = 3;
	indices[i++] = 3;	indices[i++] = 6;	indices[i++] = 7;

	indices[i++] = 3;	indices[i++] = 7;	indices[i++] = 0;
	indices[i++] = 0;	indices[i++] = 7;	indices[i++] = 4;

	if (combinedStyle.BorderColor.a != 255)
		m->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(m);
	//toeTransformScreenSpace3D(v,v+vertices,renderContext->transformation, renderContext->viewportSize);
	IwGxSetVertStreamScreenSpace(v,vertices);
	IwGxSetColStream(col);
	IwGxDrawPrims(IW_GX_TRI_LIST, indices, i);
}
//Render image on the screen surface
void CsmItem::Render(smItemContext* renderContext)
{
	if (!IsVisible(renderContext))
		return;
	smItemContext context = *renderContext;
	context.parentStyle = &combinedStyle;

	RenderShadow(renderContext);
	RenderBackgroud(renderContext);
	RenderBorder(renderContext);

	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Render(&context);
	}
}
void CsmItem::CombineStyle(smItemContext* renderContext)
{
	InheritStyle(renderContext->parentStyle);
	ApplyStyleSheet(renderContext->styleSheet);
	ApplyStyle(&style);
}
void CsmItem::InheritStyle(CsmStyleSettings* parentSettings)
{
	combinedStyle.Inherit(parentSettings);
}
void CsmItem::ApplyStyleSheet(CsmStyleSheet* styleSheet)
{
	if (styleSheet)
		styleSheet->Apply(&combinedStyle,GetElementNameHash(),GetElementClassHash(),GetElementStateHash());
}
void CsmItem::ApplyStyle(CsmStyle* style)
{
	style->Apply(&combinedStyle);
}


bool CsmItem::VisitForward(IsmVisitor* visitor)
{
	if (!visitor->Visited(this))
		return false;
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		if (!item->VisitForward(visitor))
			return false;
	}
	return true;
}
bool CsmItem::VisitBackward(IsmVisitor* visitor)
{
	CIwManaged** i = childItems.GetEnd();
	for (; i!=childItems.GetBegin();)
	{
		--i;
		CsmItem* item = static_cast<CsmItem*>(*i);
		if (!item->VisitForward(visitor))
			return false;
	}
	return visitor->Visited(this);
}


CsmItem* CsmItem::FindActiveItemAt(const CIwVec2 & pos)
{
	if (IsActive())
	{
		if (pos.x >= origin.x && pos.y >= origin.y && pos.x < origin.x+size.x && pos.y < origin.y+size.y)
			return this;
	}
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		CsmItem* foundItem = item->FindActiveItemAt(pos);
		if (foundItem)
			return foundItem;
	}
	return 0;
}
void CsmItem::SetFocus(bool f)
{
	if (f)
		state = IwHashString("Focus");
	else
		state = SM_ANYSTYLE;
}
void CsmItem::Touch(smTouchContext* touchContext)
{
}
void CsmItem::TouchReleased(smTouchContext* touchContext)
{
}
void CsmItem::TouchMotion(smTouchContext* touchContext)
{
}
uint32 CsmItem::GetElementNameHash() { return SM_ANYSTYLE; }
uint32 CsmItem::GetElementClassHash() { return styleClass; }
uint32 CsmItem::GetElementStateHash() { return state; }
void CsmItem::SendLazyEvent(CsmLazyEvent*e)
{
	lazyEvents.AttachTail(e);
	GetRoot()->lazyEvents.AttachTail(e);
}
#ifdef IW_BUILD_RESOURCES
//Parses from text file: start block.
void	CsmItem::ParseOpen(CIwTextParserITX* pParser)
{
	CIwManaged::ParseOpen(pParser);
}


//Parses from text file: parses attribute/value pair.
bool	CsmItem::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("style", pAttrName))
	{
		pParser->PushObject(&style);
		return true;
	}
	if (!stricmp("styleClass", pAttrName))
	{
		pParser->ReadStringHash(&styleClass);
		return true;
	}
	if (!stricmp("state", pAttrName))
	{
		pParser->ReadStringHash(&state);
		return true;
	}
	if (!stricmp("text", pAttrName))
	{
		CsmTextBlock* tb = new CsmTextBlock(pParser->ReadString());
		childItems.Add(tb);
		return true;
	}
	if (!stricmp("id", pAttrName))
	{
		pParser->ReadStringHash(&idHash);
		return true;
	}
	
	if (!stricmp("image", pAttrName))
	{
		uint32 t;
		pParser->ReadStringHash(&t);
		CsmImage* ti = new CsmImage(t);
		childItems.Add(ti);
		return true;
	}
	if (!stricmp(pAttrName, "onupdate"))
	{
		smReadString(pParser, &onUpdate);
		return true;
	}
	return CIwManaged::ParseAttribute(pParser, pAttrName);
}
//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
void	CsmItem::ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild)
{
	//CIwManaged::ParseCloseChild(pParser, pChild);
	if (&style == (CsmStyle*)pChild)
		return;
	childItems.Add(pChild);
}
#endif
