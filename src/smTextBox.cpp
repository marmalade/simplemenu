#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include <s3eOSReadString.h>
#include "smTextBox.h"
#include "smMenu.h"
#include "simplemenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmTextBox);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmTextBox);

//Constructor
CsmTextBox::CsmTextBox()
{
	utf8string = 0;
	//cachedWithCombinedStyle;
	cachedWithWidth = -100;
}
//Constructor
CsmTextBox::CsmTextBox(char*buf)
{
	utf8string = buf;
}
//Desctructor
CsmTextBox::~CsmTextBox()
{
	if (utf8string)
	{
		delete [] utf8string;
		utf8string= 0;
	}
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmTextBox::Serialise ()
{
	CsmTerminalItem::Serialise();
	smSerialiseString(onChanged);

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
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmTextBox::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmTextBox> d ("CsmTextBox",
			ScriptTraits::Method("GetRoot", &CsmTextBox::GetRoot),
			ScriptTraits::Method("GetText", &CsmTextBox::GetText),
			ScriptTraits::Method("SetText", &CsmTextBox::SetText),
			0);
	return &d;
}
const char* CsmTextBox::GetText() const
{
	return utf8string;
}
void CsmTextBox::SetText(const char* t)
{
	if (!t && !utf8string)
		return;
	bool changed = false;
	if (t && utf8string)
		if (!strcmp(t,utf8string))
			changed = true;
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
	if (changed)
		OnTextChanged();
}

void CsmTextBox::TouchReleased(smTouchContext* touchContext)
{
	const char* s= s3eOSReadStringUTF8WithDefault("",utf8string);
	if (s)
		SetText(s);
}

void CsmTextBox::Prepare(smItemContext* renderContext,int16 width)
{
	EvalUpdate();
	CombineStyle(renderContext);
	if (cachedWithWidth == width && combinedStyle == cachedWithCombinedStyle)
		return;
	cachedWithCombinedStyle = combinedStyle;
	cachedWithWidth = width;
	FreeTypeHelper::CfthFont* f = combinedStyle.Font;
	if (!f)
		return;
	int16 contentWidth = width - GetMarginLeft() - GetMarginRight() - GetPaddingLeft() - GetPaddingRight();
	CIwArray<FreeTypeHelper::CfthGlyphLayout> layout;
	layoutData.origin = CIwSVec2::g_Zero;
	layoutData.shadowOffset = CIwSVec2(combinedStyle.TextShadowOffset.x.GetPx(1),combinedStyle.TextShadowOffset.y.GetPx(1));
	layoutData.shadowColour = combinedStyle.TextShadowColor;
	layoutData.size.x = contentWidth;
	layoutData.size.y = combinedStyle.FontSize.GetPx(width);
	layoutData.textAlignment = combinedStyle.HorizontalAlignment;//IW_GEOM_ONE/3;
	layoutData.isRightToLeft = false;//CtoeFreeTypeFont::IsRightToLeft();
	layoutData.actualSize.y = 0;
	layoutData.actualSize.x = width;
	if (utf8string)
	{
		f->LayoutGlyphs(utf8string, layoutData);
	}

	size.x = width;
	size.y = layoutData.actualSize.y + GetMarginTop()+GetMarginBottom()+GetPaddingTop()+GetPaddingBottom();
}
//Render image on the screen surface
void CsmTextBox::Render(smItemContext* renderContext)
{
	if (!IsVisible(renderContext)) return;
	RenderShadow(renderContext);
	RenderBackgroud(renderContext);
	RenderBorder(renderContext);
	CIwSVec2 p = GetOrigin()+CIwSVec2(GetMarginLeft()+GetPaddingLeft(),GetMarginTop()+GetPaddingTop());
	layoutData.RenderAt(p,renderContext->viewportSize,renderContext->transformation,combinedStyle.FontColor);
	
}
void CsmTextBox::RearrangeChildItems()
{
	CsmTerminalItem::RearrangeChildItems();
}
void CsmTextBox::OnTextChanged()
{
	if (onChanged.size() != 0 && GetRoot())
		GetRoot()->Eval(this, onChanged.c_str());
}

#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmTextBox::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("text",pAttrName))
	{
		utf8string = pParser->ReadString();
		return true;
	}
	if (!stricmp("onchange",pAttrName))
	{
		smReadString(pParser, &onChanged);
		return true;
	}

	return CsmTerminalItem::ParseAttribute(pParser, pAttrName);
}

#endif
