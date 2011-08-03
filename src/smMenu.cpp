#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "simplemenu.h"
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
}
//Desctructor
CsmMenu::~CsmMenu()
{
	childItems.Delete();
}
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmMenu::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmMenu> d ("CsmMenu",
			ScriptTraits::Method("GetContent", &CsmMenu::GetContent),
			ScriptTraits::Method("GetHeader", &CsmMenu::GetHeader),
			ScriptTraits::Method("GetFooter", &CsmMenu::GetFooter),
			ScriptTraits::Method("GetItemById", &CsmMenu::GetItemById),
			0);
	return &d;
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmMenu::Serialise ()
{
	CIwResource::Serialise();
	childItems.Serialise();
	style.Serialise();
	IwSerialiseUInt32(styleSheetHash);
	smSerialiseString(onUpdate);
	smSerialiseString(onLoad);
	if (IwSerialiseIsReading())
	{
		if (styleSheetHash)
		{
			styleSheet = (CsmStyleSheet*)IwGetResManager()->GetResHashed(styleSheetHash,"CsmStyleSheet");
		}
		for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
		{
			CsmItem* item = static_cast<CsmItem*>(*i);
			item->InitTree(this,0);
		}
	}
}
//Render image on the screen surface
void CsmMenu::Render()
{
	smItemContext renderContext;
	if (styleSheet)
		styleSheet->Apply(&styleSettings, IwHashString("SIMPLEMENU"), SM_ANYSTYLE, SM_ANYSTYLE);
	style.Apply(&styleSettings);
	renderContext.parentStyle = &styleSettings;
	renderContext.styleSheet = styleSheet;
	renderContext.viewportPos = CIwSVec2::g_Zero;
	//renderContext.font = (CtoeFreeTypeFont*)IwGetResManager()->GetResNamed("Steinerlight","CtoeFreeTypeFont");
	//renderContext.font = (CtoeFreeTypeFont*)IwGetResManager()->GetResNamed("font","CtoeFreeTypeFont");
	int16 w = IwGxGetScreenWidth();
	int16 h = IwGxGetScreenHeight();
	renderContext.viewportSize = CIwSVec2(w,h);
	//renderContext.transformation.SetRotY(IW_GEOM_ONE/16);
	styleSettings.Background.Render(CIwSVec2::g_Zero,CIwSVec2(w,h), renderContext.viewportSize, renderContext.transformation);
	
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Prepare(&renderContext,w);
	}
	AlignBlocks();
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Render(&renderContext);
	}

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
				smDrawSimpleMenuScrollbar(CIwSVec2(renderContext.viewportSize.x-4,contentAreaOffset-1),CIwSVec2(4,contentAreaHeight+2),
					CIwSVec2(renderContext.viewportSize.x-3,contentAreaOffset+stop),CIwSVec2(2,sbottom-stop));
			}
		}
	}
}
void CsmMenu::Update(iwfixed dt)
{
	CsmItem* content=GetContent();

	if (!activeTouch)
	{
		if (content)
		{
			int16 contentHeight = content->GetSize().y;
			int16 minContent = 0;
			int16 maxContent = 0;
			if (contentHeight > contentAreaHeight)
			{
				minContent = contentAreaHeight-contentHeight;
			}
			else
			{
				maxContent = minContent = (contentAreaHeight-contentHeight)/2;
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
				scrollAnimation = scrollAnimation*8/10;
			}
			content->SetOrigin(CIwSVec2(0,contentAreaOffset+contentOffset));
		}
	}
	else
	{
		scrollAnimation = scrollAnimationAcc;
		scrollAnimationAcc = 0;
		content->SetOrigin(CIwSVec2(0,contentAreaOffset+contentOffset));
	}

	while (lazyEvents.GetFirstChild())
	{
		CsmLazyEvent* e = lazyEvents.GetFirstChild();
		((TsmIntrusiveListItem<CsmLazyEvent>*)e)->Detach();
		((TsmIntrusiveListItem<CsmLazyEvent,CsmMenu>*)e)->Detach();		
		e->Send();
		delete e;
	}

	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		item->Animate(dt);
	}

	if (onUpdate.size() > 0)
		if (scriptProvider)
			scriptProvider->Eval(onUpdate.c_str(), this, this->GetInstanceClassDescription());


}
void CsmMenu::AlignBlocks()
{
	//CsmItem* content=GetContent();
	CsmItem* header=GetHeader();
	CsmItem* footer=GetFooter();

	contentAreaHeight = (int16)IwGxGetScreenHeight();
	contentAreaOffset =0;
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
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		if (!item->VisitForward(visitor))
			return false;
	}
	return true;
}
bool CsmMenu::VisitBackward(IsmVisitor* visitor) const
{
	CIwManaged** i = childItems.GetEnd();
	for (; i!=childItems.GetBegin(); )
	{
		--i;
		CsmItem* item = static_cast<CsmItem*>(*i);
		if (!item->VisitBackward(visitor))
			return false;
	}
	return true;
}

CsmItem* CsmMenu::FindActiveItemAt(const CIwVec2 & coord)
{
	for (CIwManaged** i = childItems.GetBegin(); i!=childItems.GetEnd(); ++i)
	{
		CsmItem* item = static_cast<CsmItem*>(*i);
		CsmItem* foundItem = item->FindActiveItemAt(coord);
		if (foundItem)
			return foundItem;
	}
	return 0;
}
bool CsmMenu::KeyEvent(smKeyContext* keyContext)
{
	if (keyContext->pressed)
	{
		switch (keyContext->key)
		{
		case s3eKeyUp:
			{
				CsmNextActive v(activeItem);
				VisitBackward(&v);
				if (v.m_found)
				{
					SetFocusTo(v.m_found);
					ScrollToItem(v.m_found);
				}
			}
			break;
		case s3eKeyDown:
			{
				CsmNextActive v(activeItem);
				VisitForward(&v);
				if (v.m_found)
				{
					SetFocusTo(v.m_found);
					ScrollToItem(v.m_found);
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
	}
	return true;
}
void CsmMenu::ScrollToItem(CsmItem*i)
{
	CsmItem*p=i;
	while(p->GetParent()) p = p->GetParent();
	CsmItem*content=GetContent();
	if (p == content)
	{
		int16 contentHeight = content->GetSize().y;
		if (contentHeight > contentAreaHeight)
		{
			int16 minContent = contentAreaHeight-contentHeight;
			int16 curY = i->GetSize().y/2 + i->GetOrigin().y-contentAreaOffset;
			contentOffset = -curY+contentAreaHeight/2;
			if (contentOffset > 0)
				contentOffset = 0;
			else if (contentOffset < minContent)
				contentOffset = minContent;
		}
	}
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
	DPI::CdpiLength threshold(10*IW_GEOM_ONE, DPI::CdpiLength::PT);
	if (isVerticalScrolled || abs(touchContext->firstKnownPoistion.y - touchContext->currentPoistion.y) > threshold.GetPx(1))
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
	childItems.Add(pChild);
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