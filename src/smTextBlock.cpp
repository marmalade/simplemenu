#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smTextBlock.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmTextBlock);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmTextBlock);

//Constructor
CsmTextBlock::CsmTextBlock()
{
	utf8string = 0;
	//cachedWithCombinedStyle;
	cachedWithWidth = -100;
}
//Constructor
CsmTextBlock::CsmTextBlock(char*buf)
{
	utf8string = buf;
}
//Desctructor
CsmTextBlock::~CsmTextBlock()
{
	if (utf8string)
	{
		delete [] utf8string;
		utf8string= 0;
	}
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmTextBlock::Serialise ()
{
	CsmTerminalItem::Serialise();
	style.Serialise();
	size_t len = 0;
	if (IwSerialiseIsReading())
	{
		IwSerialiseUInt32(len);
		if (len)
		{
			utf8string = new char[len+1];
			IwSerialiseString(utf8string);
		}
	}
	else
	{
		if (utf8string)
		{
			len = strlen(utf8string);
			IwSerialiseUInt32(len);
			if (len > 0)
				IwSerialiseString(utf8string);
		}
		else
		{
			IwSerialiseUInt32(len);
		}
	}
}
uint32 CsmTextBlock::GetElementNameHash()
{
	static uint32 name = IwHashString("TextBlock");
	return name;
}
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmTextBlock::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmTextBlock> d (CsmTerminalItem::GetClassDescription(), "CsmTextBlock",
			ScriptTraits::Method("GetText", &CsmTextBlock::GetText),
			ScriptTraits::Method("SetText", &CsmTextBlock::SetText),
			0);
	return &d;
}
const char* CsmTextBlock::GetText() const
{
	return utf8string;
}
void CsmTextBlock::SetText(const char* t)
{
	cachedWithCombinedStyle = CsmStyleSettings();
	if (utf8string)
	{
		delete utf8string;
		utf8string = 0;
	}
	if (t)
	{
		utf8string = new char[strlen(t)+1];
		strcpy(utf8string,t);
	}
}
void CsmTextBlock::CollectInnerTextTo(std::stringstream & s) const
{
	if (utf8string)
		s << utf8string;
}
void CsmTextBlock::Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize)
{
	EvalUpdate();
	CombineStyle(renderContext);
	if (cachedWithWidth == recommendedSize.x && combinedStyle == cachedWithCombinedStyle)
		return;
	cachedWithCombinedStyle = combinedStyle;
	cachedWithWidth = recommendedSize.x;
	FreeTypeHelper::CfthFont* f = combinedStyle.Font;
	if (!f)
		return;
	int16 contentWidth = recommendedSize.x - GetMarginLeft() - GetMarginRight() - GetPaddingLeft() - GetPaddingRight();
	CIwArray<FreeTypeHelper::CfthGlyphLayout> layout;
	layoutData.origin = CIwSVec2::g_Zero;
	layoutData.shadowOffset = CIwSVec2(combinedStyle.TextShadowOffset.x.GetPx(1),combinedStyle.TextShadowOffset.y.GetPx(1));
	layoutData.shadowColour = combinedStyle.TextShadowColor;
	layoutData.size.x = contentWidth;
	layoutData.size.y = combinedStyle.FontSize.GetPx(recommendedSize.y);
	layoutData.textAlignment = combinedStyle.HorizontalAlignment;//IW_GEOM_ONE/3;
	layoutData.isRightToLeft = false;//CtoeFreeTypeFont::IsRightToLeft();
	layoutData.actualSize.y = 0;
	layoutData.actualSize.x = recommendedSize.x;
	if (utf8string)
	{
		f->LayoutGlyphs(utf8string, layoutData);
	}

	size.x = recommendedSize.x;
	size.y = layoutData.actualSize.y + GetMarginTop()+GetMarginBottom()+GetPaddingTop()+GetPaddingBottom();
}
//Render image on the screen surface
void CsmTextBlock::Render(smItemContext* renderContext)
{
	if (!IsVisible(renderContext)) return;
	if (combinedStyle.ShadowSize.Value >= 0)
		RenderShadow(renderContext);
	RenderBackgroud(renderContext);
	RenderBorder(renderContext);
	if (combinedStyle.ShadowSize.Value < 0)
		RenderInternalShadow(renderContext);
	CIwSVec2 p = GetOrigin()+CIwSVec2(GetMarginLeft()+GetPaddingLeft(),GetMarginTop()+GetPaddingTop());
	layoutData.RenderAt(p,renderContext->viewportSize,renderContext->transformation,combinedStyle.FontColor);
	
}
void CsmTextBlock::RearrangeChildItems()
{
	CsmTerminalItem::RearrangeChildItems();
}
#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmTextBlock::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("text",pAttrName))
	{
		utf8string = pParser->ReadString();
		return true;
	}
	return CsmTerminalItem::ParseAttribute(pParser, pAttrName);
}

#endif
