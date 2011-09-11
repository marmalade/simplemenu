#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "simplemenu.h"
#include "math.h"
#include "smMenu.h"
#include "smItem.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	class CsmFindById: public IsmVisitor
	{
		public:
		CsmItem* m_found;
		uint32 m_hash;
		CsmFindById(uint32 h):m_found(0),m_hash(h){}
		virtual bool Visited(CsmItem*i) {
			if (i->GetElementIdHash() == m_hash)
			{
				m_found = i;
				return false;
			}
			return true;
		}
	};
	class CsmNextActive: public IsmVisitor
	{
	public:
		CsmItem* m_skip;
		CsmItem* m_found;

		CsmNextActive(CsmItem* skip):m_skip(skip),m_found(0){}
		virtual bool Visited(CsmItem*i) {
			if (!m_skip && i->IsActive())
			{
				m_found = i;
				return false;
			}
			if (m_skip == i)
				m_skip = 0;
			return true;
		}

	};
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmMenu);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmMenu);

//Constructor
CsmMenu::CsmMenu()
{
	styleSheet= 0;
	styleSheetHash = 0;
	contentOffset = 0;
	contentAreaHeight = 100;
	contentAreaOffset = 0;
	scrollAnimationAcc = 0;
	activeTouch = 0;
	activeItem = 0;
	isVerticalScrolled = false;
	material = 0;
	blendMaterial = 0;
	isHeaderScrollable = false;
	isPreparedToRender = false;
	transition = 0;
}
//Desctructor
CsmMenu::~CsmMenu()
{
	if (material)
		delete material;
	if (blendMaterial)
		delete blendMaterial;
	if (content.item)
		delete content.item;
	if (header.item)
		delete header.item;
	if (footer.item)
		delete footer.item;
	if (overlay.item)
		delete overlay.item;
}
CIwMaterial* CsmMenu::GetFlatMaterial()
{
	if (material)
		return material;
	material = new CIwMaterial();
	material->SetColAmbient(255,255,255,255);
	material->SetColEmissive(255,255,255,255);
	material->SetColDiffuse(255,255,255,255);
	material->SetModulateMode(CIwMaterial::MODULATE_RGB);
	return material;
}
CIwMaterial* CsmMenu::GetBlendedMaterial()
{
	if (blendMaterial)
		return blendMaterial;
	blendMaterial = new CIwMaterial();
	blendMaterial->SetColAmbient(255,255,255,255);
	blendMaterial->SetColEmissive(255,255,255,255);
	blendMaterial->SetColDiffuse(255,255,255,255);
	blendMaterial->SetModulateMode(CIwMaterial::MODULATE_RGB);
	blendMaterial->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	return blendMaterial;
}
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmMenu::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmMenu> d (0, "CsmMenu",
			ScriptTraits::Method("GetContent", &CsmMenu::GetContent),
			ScriptTraits::Method("GetHeader", &CsmMenu::GetHeader),
			ScriptTraits::Method("GetFooter", &CsmMenu::GetFooter),
			ScriptTraits::Method("GetItemById", &CsmMenu::GetItemById),
			ScriptTraits::Method("GetInnerText", &CsmMenu::GetInnerText),
			
			0);
	return &d;
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmMenu::Serialise ()
{
	CIwResource::Serialise();
	smSerialiseManaged(content.item);
	smSerialiseManaged(header.item);
	smSerialiseManaged(footer.item);
	smSerialiseManaged(overlay.item);
	style.Serialise();
	IwSerialiseUInt32(styleSheetHash);
	smSerialiseString(onUpdate);
	smSerialiseString(onLoad);
	if (IwSerialiseIsReading())
	{
		SetStyleSheetHash(styleSheetHash);

		if (content.item) content.item->OnAttachToMenu(this,0);
		if (header.item) header.item->OnAttachToMenu(this,0);
		if (footer.item) footer.item->OnAttachToMenu(this,0);
		if (overlay.item) overlay.item->OnAttachToMenu(this,0);
	}
}
void CsmMenu::SetStyleSheetHash(uint32 v) 
{
	styleSheetHash = v;
	if (styleSheetHash)
	{
		styleSheet = (CsmStyleSheet*)IwGetResManager()->GetResHashed(styleSheetHash,"CsmStyleSheet");
	}
}
void SetScaleAt(iwfixed scaleX,iwfixed scaleY,int x,int y, CIwMat2D* m)
{
	m->m[0][0] = scaleX;
	m->m[0][1] = 0;
	m->m[1][0] = 0;
	m->m[1][1] = scaleY;
	m->t.x = x - x*scaleX/IW_GEOM_ONE;
	m->t.y = y - y*scaleY/IW_GEOM_ONE;

	//m.t.y = y - h*(IW_GEOM_ONE-scaleY)/IW_GEOM_ONE/2;
}
//Render image on the screen surface
void CsmMenu::Prepare()
{
	isPreparedToRender = true;
	smItemContext renderContext = smItemContext();
	
	if (styleSheet)
		styleSheet->Apply(&styleSettings, IwHashString("SIMPLEMENU"), SM_ANYSTYLE, SM_ANYSTYLE);
	style.Apply(&styleSettings);
	renderContext.parentStyle = &styleSettings;
	renderContext.styleSheet = styleSheet;
	renderContext.viewportPos = CIwSVec2::g_Zero;
	int16 w = IwGxGetScreenWidth();
	int16 h = IwGxGetScreenHeight();
	CIwSVec2 recommendedSize = renderContext.viewportSize = CIwSVec2(w,h);
	styleSettings.Background.Render(CIwSVec2::g_Zero,CIwSVec2(w,h), renderContext.viewportSize, renderContext.transformation);

	header.renderContext = footer.renderContext = content.renderContext = overlay.renderContext = renderContext;
	if (transition != 0)
	{
		iwfixed t = abs(transition);
		iwfixed scale = IW_GEOM_ONE-t;

		//header.renderContext.transformation.SetRot(-t/4,CIwVec2(0,0));
		SetScaleAt(scale,scale,w/2,0,&header.renderContext.transformation);

		//footer.renderContext.transformation.SetRot(-t/4,CIwVec2(w,h));
		SetScaleAt(scale,scale,w/2,h,&footer.renderContext.transformation);

		SetScaleAt(scale,scale,w/2,h/2,&content.renderContext.transformation);
		SetScaleAt(scale,scale,w/2,h/2,&overlay.renderContext.transformation);
	}

	int16 contentAreaLeftover = h;
	CsmItem* item;
	item = GetHeader();
	if (item) { item->Prepare(&header.renderContext,recommendedSize); contentAreaLeftover -= item->GetSize().y; }
	item = GetFooter();
	if (item) { item->Prepare(&footer.renderContext,recommendedSize); contentAreaLeftover -= item->GetSize().y; }
	isHeaderScrollable = contentAreaLeftover < h/3;
	item = GetContent();
	if (item) 
	{
		recommendedSize.y = (isHeaderScrollable)?h:contentAreaLeftover;
		item->Prepare(&content.renderContext,recommendedSize);
	}

	AlignBlocks();
}
//Render image on the screen surface
void CsmMenu::Render()
{
	if (!isPreparedToRender)
		Prepare();

	CsmItem* item;

	// Render Content
	item = GetContent();
	if (item) { item->Render(&content.renderContext);}
	// Render Header
	item = GetHeader();
	if (item) { item->Render(&header.renderContext);}
	// Render Footer
	item = GetFooter();
	if (item) { item->Render(&footer.renderContext);}

	//render scroll
	CsmItem* content=GetContent();
	if ((activeTouch || scrollAnimation) && content)
	{
		int16 contentHeight = content->GetSize().y;
		if (contentHeight)
		{
			int16 contentVisibleTop = -contentOffset;
			
			int16 contentVisibleBottom = contentVisibleTop + contentAreaHeight;
			if (contentVisibleTop < 0)
				contentVisibleTop = 0;
			if (contentVisibleTop > contentHeight)
				contentVisibleTop = contentHeight;
			if (contentVisibleBottom < 0)
				contentVisibleBottom = 0;
			if (contentVisibleBottom > contentHeight)
				contentVisibleBottom = contentHeight;
			if (contentVisibleTop <  contentVisibleBottom)
			{
				int16 stop = contentVisibleTop*contentAreaHeight/contentHeight;
				int16 sbottom = contentVisibleBottom*contentAreaHeight/contentHeight;
				smDrawSimpleMenuScrollbar(CIwSVec2(overlay.renderContext.viewportSize.x-4,contentAreaOffset-1),CIwSVec2(4,contentAreaHeight+2),
					CIwSVec2(overlay.renderContext.viewportSize.x-3,contentAreaOffset+stop),CIwSVec2(2,sbottom-stop));
			}
		}
	}

	isPreparedToRender = false;
}
void CsmMenu::Update(iwfixed dt)
{
	if (!activeTouch)
	{
		if (content.item)
		{
			int16 contentHeight = content.item->GetSize().y;
			int16 minContent = 0;
			int16 maxContent = 0;
			if (isHeaderScrollable)
			{
				maxContent = header.item?header.item->GetSize().y:0;
				minContent = contentAreaHeight-( (content.item?content.item->GetSize().y:0) + (footer.item?footer.item->GetSize().y:0));
			}
			else
			{
				if (contentHeight > contentAreaHeight)
				{
					minContent = contentAreaHeight-contentHeight;
				}
				else
				{
					maxContent = minContent = (contentAreaHeight-contentHeight)/2;
				}
			}
			if (contentOffset < minContent)
			{
				contentOffset = minContent+(contentOffset-minContent)/2;
				scrollAnimation = 0;
			}
			else if (contentOffset > maxContent)
			{
				contentOffset = maxContent+(contentOffset-maxContent)/2;
				scrollAnimation = 0;
			}
			else
			{
				contentOffset += scrollAnimation;
				scrollAnimation = scrollAnimation*9/10;
			}
			content.item->SetOrigin(CIwSVec2(0,contentAreaOffset+contentOffset));
		}
	}
	else
	{
		scrollAnimation = scrollAnimationAcc;
		scrollAnimationAcc = 0;
		content.item->SetOrigin(CIwSVec2(0,contentAreaOffset+contentOffset));
	}

	while (lazyEvents.GetFirstChild())
	{
		CsmLazyEvent* e = lazyEvents.GetFirstChild();
		((TsmIntrusiveListItem<CsmLazyEvent>*)e)->Detach();
		((TsmIntrusiveListItem<CsmLazyEvent,CsmMenu>*)e)->Detach();		
		e->Send();
		delete e;
	}

	if (content.item) content.item->Animate(dt);
	if (header.item) header.item->Animate(dt);
	if (footer.item) footer.item->Animate(dt);
	if (overlay.item) overlay.item->Animate(dt);

	if (onUpdate.size() > 0)
		if (scriptProvider)
			scriptProvider->Eval(onUpdate.c_str(), this, this->GetInstanceClassDescription());


}
void CsmMenu::AlignBlocks()
{
	CsmItem* content=GetContent();
	CsmItem* header=GetHeader();
	CsmItem* footer=GetFooter();

	contentAreaHeight = (int16)IwGxGetScreenHeight();
	contentAreaOffset = 0;

	if (isHeaderScrollable)
	{
		if (header)
		{
			header->SetOrigin(CIwSVec2(0,contentOffset-header->GetSize().y));
		}
		if (footer)
		{
			footer->SetOrigin(CIwSVec2(0,contentOffset+((content)?content->GetSize().y:0)));
		}
	}
	else
	{
		if (header)
		{
			header->SetOrigin(CIwSVec2(0,0));
			contentAreaOffset += header->GetSize().y;
			contentAreaHeight -= header->GetSize().y;
		}
		if (footer)
		{
			footer->SetOrigin(CIwSVec2(0,contentAreaOffset+contentAreaHeight-footer->GetSize().y));
			contentAreaHeight -= footer->GetSize().y;
		}
	}

}
CsmItem* CsmMenu::GetItemById(const char*s) const
{
	return GetItemByHash(IwHashString(s));
}
CsmItem* CsmMenu::GetItemByHash(uint32 h) const
{
	CsmFindById v(h);
	VisitForward(&v);
	return v.m_found;
}

bool CsmMenu::VisitForward(IsmVisitor* visitor) const
{
	if (content.item) if (!content.item->VisitForward(visitor)) return false;
	if (header.item) if (!header.item->VisitForward(visitor)) return false;
	if (footer.item) if (!footer.item->VisitForward(visitor)) return false;
	if (overlay.item) if (!overlay.item->VisitForward(visitor)) return false;
	return true;
}
bool CsmMenu::VisitBackward(IsmVisitor* visitor) const
{
	if (overlay.item) if (!overlay.item->VisitBackward(visitor)) return false;
	if (footer.item) if (!footer.item->VisitBackward(visitor)) return false;
	if (header.item) if (!header.item->VisitBackward(visitor)) return false;
	if (content.item) if (!content.item->VisitBackward(visitor)) return false;
	return true;
}

CsmItem* CsmMenu::FindActiveItemAt(const CIwVec2 & coord)
{
	CsmItem* foundItem = 0;
	if (overlay.item) if (foundItem = overlay.item->FindActiveItemAt(coord)) return foundItem;
	if (footer.item) if (foundItem = footer.item->FindActiveItemAt(coord)) return foundItem;
	if (header.item) if (foundItem = header.item->FindActiveItemAt(coord)) return foundItem;
	if (content.item) if (foundItem = content.item->FindActiveItemAt(coord)) return foundItem;
	return 0;
}
bool CsmMenu::KeyReleasedEvent(smKeyContext* keyContext)
{
	if (keyContext->receiver == this && keyContext->receiverContextPointer)
	{
		if (((CsmItem*)keyContext->receiverContextPointer)->KeyReleasedEvent(keyContext))
			return true;
	}
	return false;
}
bool CsmMenu::KeyPressedEvent(smKeyContext* keyContext)
{
	if (activeItem)
	{
		if (activeItem->KeyPressedEvent(keyContext))
		{
			keyContext->receiverContextPointer = activeItem;
			return true;
		}
	}
	switch (keyContext->key)
	{
	case s3eKeyUp:
		{
			CsmNextActive v(activeItem);
			VisitBackward(&v);
			if (v.m_found)
			{
				if (ScrollToItem(v.m_found))
					SetFocusTo(v.m_found);
			}
		}
		break;
	case s3eKeyDown:
		{
			CsmNextActive v(activeItem);
			VisitForward(&v);
			if (v.m_found)
			{
				if (ScrollToItem(v.m_found))
					SetFocusTo(v.m_found);
			}
		}
		break;
	case s3eKeyEnter:
	case s3eKeyOk:
		{
			CsmItem* skip = activeItem;
			if (skip)
			{
				smTouchContext c;
				c.touchID = 0;
				c.lastKnownPoistion = c.firstKnownPoistion = c.currentPoistion = CIwSVec2::g_Zero;
				skip->Touch(&c);
				skip->TouchReleased(&c);
			}
		}
		break;
	default:
		break;
	}
	return true;
}
bool CsmMenu::ScrollToItem(CsmItem*i)
{
	CsmItem*p=i;
	while(p->GetParent()) p = p->GetParent();
	CsmItem*content=GetContent();
	CsmItem*footer=GetFooter();
	CsmItem*header=GetHeader();
	int16 itemPosition = i->GetOrigin().y;

	if (p == content)
	{
		int16 contentHeight = content->GetSize().y;
		if (contentHeight > contentAreaHeight)
		{
			int16 minContent = contentAreaHeight-contentHeight;
			int16 curY = i->GetSize().y/2 + i->GetOrigin().y-contentAreaOffset;
			int16 newContentOffset = -curY+contentAreaHeight/2;
			if (abs(contentOffset-newContentOffset) > contentAreaHeight/2)
			{
				if (newContentOffset < contentOffset)
					contentOffset -= contentAreaHeight/2;
				else
					contentOffset += contentAreaHeight/2;
				return false;
			}
			contentOffset = newContentOffset;
			if (contentOffset > 0)
				contentOffset = 0;
			else if (contentOffset < minContent)
				contentOffset = minContent;
		}
	}
	return true;
}
void CsmMenu::Initialize(IsmScriptProvider* sp)
{
	scriptProvider = sp;
	if ((onLoad.size() > 0) && scriptProvider)
		scriptProvider->Eval(onLoad.c_str(), this, this->GetInstanceClassDescription());
}
void CsmMenu::Eval(CsmItem*item, const char*s)
{
	if (!s || !*s)
		return;
	if (scriptProvider)
		scriptProvider->Eval(s, item,item->GetInstanceClassDescription());
}
void CsmMenu::SetFocusTo(CsmItem* i)
{
	if (activeItem == i)
		return;
	if (activeItem)
		activeItem->SetFocus(false);
	activeItem = i;
	if (activeItem)
		activeItem->SetFocus(true);
}
bool CsmMenu::TouchEvent(smTouchContext* touchContext)
{
	activeTouch = touchContext;
	if (scrollAnimation != 0)
	{
		isVerticalScrolled = true;
		if (activeItem)
		{
			SetFocusTo(0);
			activeItem = 0;
		}
	}
	else
	{
		CsmItem* i = FindActiveItemAt(touchContext->currentPoistion);
		if (i)
		{
			if (activeItem != i)
			{
				SetFocusTo(i);
				i->Touch(touchContext);
			}
		}
	}
	return true;
}
bool CsmMenu::TouchReleaseEvent(smTouchContext* touchContext)
{
	if (activeTouch == touchContext)
		activeTouch = 0;
	if (activeItem)
	{
		activeItem->TouchReleased(touchContext);
		activeItem->SetFocus(false);
		activeItem = 0;
	}
	isVerticalScrolled = false;
	return true;
}
bool CsmMenu::TouchMotionEvent(smTouchContext* touchContext)
{
	if (activeTouch != touchContext)
		return false;
	CIwVec2 shift = touchContext->currentPoistion-touchContext->lastKnownPoistion;
	DPI::CdpiLength threshold(16*IW_GEOM_ONE, DPI::CdpiLength::PT);
	int a = threshold.GetPx(1);
	int b = abs(touchContext->firstKnownPoistion.x - touchContext->currentPoistion.x)/4;
	int thresholdPx = (a>b)?a:b;
	
	if (isVerticalScrolled || abs(touchContext->firstKnownPoistion.y - touchContext->currentPoistion.y) > thresholdPx)
	{
		isVerticalScrolled = true;
		if (activeItem)
		{
			activeItem->TouchCanceled(touchContext);
			SetFocusTo(0);
			activeItem = 0;
		}
		scrollAnimationAcc += shift.y;
		contentOffset += shift.y;
	}
	else
	{
		if (activeItem)
			activeItem->TouchMotion(touchContext);
	}
	return true;
}

void CsmMenu::AddItem(CsmItem* item)
{
	if (!content.item) { content.item = item; item->OnAttachToMenu(this,0); return; }
	if (!header.item) { header.item = item; item->OnAttachToMenu(this,0); return; }
	if (!footer.item) { footer.item = item; item->OnAttachToMenu(this,0); return; }
	if (!overlay.item) { overlay.item = item; item->OnAttachToMenu(this,0); return; }
	IwAssertMsg(SM, false, ("Too many CsmMenu child items"));
}

std::string CsmMenu::GetInnerText() const
{
	std::stringstream s;
	CollectInnerTextTo(s);
	return s.str();
}
void CsmMenu::CollectInnerTextTo(std::stringstream & s) const
{
	if (content.item) content.item->CollectInnerTextTo(s);
	if (header.item) header.item->CollectInnerTextTo(s);
	if (footer.item) footer.item->CollectInnerTextTo(s);
	if (overlay.item) overlay.item->CollectInnerTextTo(s);
}

#ifdef IW_BUILD_RESOURCES
//Parses from text file: start block.
void	CsmMenu::ParseOpen(CIwTextParserITX* pParser)
{
	CIwResource::ParseOpen(pParser);
}
//Parses from text file: parses attribute/value pair.
bool	CsmMenu::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("styleSheet",pAttrName))
	{
		pParser->ReadStringHash(&styleSheetHash);
		return true;
	}
	if (!stricmp("style",pAttrName))
	{
		pParser->PushObject(&style);
		return true;
	}
	if (!stricmp(pAttrName, "onupdate"))
	{
		smReadString(pParser, &onUpdate);
		return true;
	}
	if (!stricmp(pAttrName, "onload"))
	{
		smReadString(pParser, &onLoad);
		return true;
	}
	return CIwResource::ParseAttribute(pParser, pAttrName);
}
//Parses from text file: end block.
void	CsmMenu::ParseClose(CIwTextParserITX* pParser)
{
	IwGetResManager()->GetCurrentGroup()->AddRes(IwHashString("CsmMenu"), this);
}
//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
void	CsmMenu::ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild)
{
	if (&style == (CsmStyle*)pChild)
		return;
	//CIwResource::ParseCloseChild(pParser, pChild);
	CsmItem* item = dynamic_cast<CsmItem*>(pChild);
	if (!item)
		IwAssertMsg(SM, false, ("CsmMenu can only have a CsmItem derivatives as child items"));
	AddItem(item);
}


IW_MANAGED_IMPLEMENT(CsmMenuResHandler)

//Constructor
CsmMenuResHandler::CsmMenuResHandler(): CIwResHandler("simplemenu", "CsmMenu")
{
	SetName("sm");
}
//Desctructor
CsmMenuResHandler::~CsmMenuResHandler()
{
}

//Buid resource method
CIwResource*	CsmMenuResHandler::Build(const CIwStringL& pathname)
{
	IwGetTextParserITX()->ParseFile(pathname.c_str());
	return NULL;
}

#endif