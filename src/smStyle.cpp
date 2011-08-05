#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smStyle.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmStyleSettings);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
//IW_MANAGED_IMPLEMENT(CsmStyleSettings);


//Reads/writes a binary file using @a IwSerialise interface.
void CsmStyleSettings::Serialise ()
{
	IwSerialiseUInt32(FontHash);
	if (IwSerialiseIsReading() && FontHash != 0)
		Font = (FreeTypeHelper::CfthFont*)IwGetResManager()->GetResHashed(FontHash,"CfthFont");
	FontSize.Serialise();
	FontColor.Serialise();
	Background.Serialise();
	Margin.Serialise();
	Padding.Serialise();
	Border.Serialise();
	BorderColor.Serialise();
	ShadowColor.Serialise();
	TextShadowColor.Serialise();
	ShadowSize.Serialise();
	ShadowOffset.Serialise();
	TextShadowOffset.Serialise();
	MinWidth.Serialise();
	MinHeight.Serialise();
	MaxWidth.Serialise();
	MaxHeight.Serialise();
	IwSerialiseBool(DropShadow);
	IwSerialiseInt32(HorizontalAlignment);
	IwSerialiseInt32(VerticalAlignment);
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmStyle::Serialise ()
{
	CIwManaged::Serialise();
	settings.Serialise();
	IwSerialiseBool(isFont);
	IwSerialiseBool(isFontSize);
	IwSerialiseBool(isFontColor);
	IwSerialiseBool(isBackground);
	IwSerialiseBool(isMargin);
	IwSerialiseBool(isPadding);
	IwSerialiseBool(isBorder);
	IwSerialiseBool(isBorderColor);
	IwSerialiseBool(isShadowColor);
	IwSerialiseBool(isTextShadowColor);
	IwSerialiseBool(isShadowOffset);
	IwSerialiseBool(isTextShadowOffset);
	IwSerialiseBool(isShadowSize);
	IwSerialiseBool(isDropShadow);
	IwSerialiseBool(isHorizontalAlignment);
	IwSerialiseBool(isVerticalAlignment);
	IwSerialiseBool(isMinWidth);
	IwSerialiseBool(isMinHeight);
	IwSerialiseBool(isMaxWidth);
	IwSerialiseBool(isMaxHeight);
}

#ifdef IW_BUILD_RESOURCES
//Parses from text file: parses attribute/value pair.
bool	CsmStyle::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("background",pAttrName))
	{
		pParser->PushObject(&settings.Background);
		isBackground = true;
		return true;
	}
	if (!stricmp("font",pAttrName))
	{
		pParser->ReadStringHash(&settings.FontHash);
		isFont = true;
		return true;
	}
	if (!stricmp("fontSize",pAttrName) || !stricmp("font-size",pAttrName))
	{
		settings.FontSize.ParseAttribute(pParser);
		isFontSize = true;
		return true;
	}
	if (!stricmp("fontColour",pAttrName) || !stricmp("fontColor",pAttrName) || !stricmp("font-colour",pAttrName) || !stricmp("font-color",pAttrName))
	{
		uint8 c[4] = {255,255,255,255};
		pParser->ReadUInt8Array(c,4);
		settings.FontColor.Set(c[0],c[1],c[2],c[3]);
		isFontColor = true;
		return true;
	}
	if (!stricmp("borderColour",pAttrName) || !stricmp("borderColor",pAttrName) || !stricmp("border-Colour",pAttrName) || !stricmp("border-Color",pAttrName))
	{
		uint8 c[4] = {255,255,255,255};
		pParser->ReadUInt8Array(c,4);
		settings.BorderColor.Set(c[0],c[1],c[2],c[3]);
		isBorderColor = true;
		return true;
	}
	if (!stricmp("shadowColour",pAttrName) || !stricmp("shadowColor",pAttrName)|| !stricmp("shadow-Colour",pAttrName) || !stricmp("shadow-Color",pAttrName))
	{
		uint8 c[4] = {255,255,255,255};
		pParser->ReadUInt8Array(c,4);
		settings.ShadowColor.Set(c[0],c[1],c[2],c[3]);
		isShadowColor = true;
		return true;
	}
	if (!stricmp("textShadowColour",pAttrName) || !stricmp("textShadowColor",pAttrName) || !stricmp("text-Shadow-Colour",pAttrName) || !stricmp("text-Shadow-Color",pAttrName))
	{
		uint8 c[4] = {255,255,255,255};
		pParser->ReadUInt8Array(c,4);
		settings.TextShadowColor.Set(c[0],c[1],c[2],c[3]);
		isTextShadowColor = true;
		return true;
	}
	if (!stricmp("HorizontalAlignment", pAttrName) || !stricmp("horizontal-alignment", pAttrName))
	{
		pParser->ReadFixed(&settings.HorizontalAlignment);
		isHorizontalAlignment = true;
		return true;
	}
	if (!stricmp("VerticalAlignment", pAttrName) || !stricmp("vertical-alignment", pAttrName))
	{
		pParser->ReadFixed(&settings.VerticalAlignment);
		isVerticalAlignment = true;
		return true;
	}
	if (!stricmp("shadow-offset", pAttrName))
	{
		settings.ShadowOffset.ParseAttribute(pParser);
		isShadowOffset = true;
		return true;
	}
	if (!stricmp("text-shadow-offset", pAttrName))
	{
		settings.TextShadowOffset.ParseAttribute(pParser);
		isTextShadowOffset = true;
		return true;
	}
	if (!stricmp("shadow-size", pAttrName))
	{
		settings.ShadowSize.ParseAttribute(pParser);
		isShadowSize = true;
		return true;
	}
	if (!stricmp("drop-shadow", pAttrName))
	{
		pParser->ReadBool(&settings.DropShadow);
		isDropShadow = true;
		return true;
	}
	if (!stricmp("margin", pAttrName))
	{
		settings.Margin.left.ParseAttribute(pParser);
		settings.Margin.top = settings.Margin.right = settings.Margin.bottom = settings.Margin.left;
		isMargin = true;
		return true;
	}
	if (!stricmp("margin-left", pAttrName))
	{
		settings.Margin.left.ParseAttribute(pParser);
		isMargin = true;
		return true;
	}
	if (!stricmp("margin-right", pAttrName))
	{
		settings.Margin.right.ParseAttribute(pParser);
		isMargin = true;
		return true;
	}
	if (!stricmp("margin-top", pAttrName))
	{
		settings.Margin.top.ParseAttribute(pParser);
		isMargin = true;
		return true;
	}
	if (!stricmp("margin-bottom", pAttrName))
	{
		settings.Margin.bottom.ParseAttribute(pParser);
		isMargin = true;
		return true;
	}

	if (!stricmp("padding", pAttrName))
	{
		settings.Padding.left.ParseAttribute(pParser);
		settings.Padding.top = settings.Padding.right = settings.Padding.bottom = settings.Padding.left;
		isPadding = true;
		return true;
	}
	if (!stricmp("padding-left", pAttrName))
	{
		settings.Padding.left.ParseAttribute(pParser);
		isPadding = true;
		return true;
	}
	if (!stricmp("padding-right", pAttrName))
	{
		settings.Padding.right.ParseAttribute(pParser);
		isPadding = true;
		return true;
	}
	if (!stricmp("padding-top", pAttrName))
	{
		settings.Padding.top.ParseAttribute(pParser);
		isPadding = true;
		return true;
	}
	if (!stricmp("padding-bottom", pAttrName))
	{
		settings.Padding.bottom.ParseAttribute(pParser);
		isMargin = true;
		return true;
	}

	if (!stricmp("border", pAttrName))
	{
		settings.Border.left.ParseAttribute(pParser);
		settings.Border.top = settings.Border.right = settings.Border.bottom = settings.Border.left;
		isBorder = true;
		return true;
	}
	if (!stricmp("border-left", pAttrName))
	{
		settings.Border.left.ParseAttribute(pParser);
		isBorder = true;
		return true;
	}
	if (!stricmp("border-right", pAttrName))
	{
		settings.Border.right.ParseAttribute(pParser);
		isBorder = true;
		return true;
	}
	if (!stricmp("border-top", pAttrName))
	{
		settings.Border.top.ParseAttribute(pParser);
		isBorder = true;
		return true;
	}
	if (!stricmp("border-bottom", pAttrName))
	{
		settings.Border.bottom.ParseAttribute(pParser);
		isBorder = true;
		return true;
	}
	if (!stricmp("min-width", pAttrName))
	{
		settings.MinWidth.ParseAttribute(pParser);
		isMinWidth = true;
		return true;
	}
	if (!stricmp("min-height", pAttrName))
	{
		settings.MinHeight.ParseAttribute(pParser);
		isMinHeight = true;
		return true;
	}
	if (!stricmp("max-width", pAttrName))
	{
		settings.MaxWidth.ParseAttribute(pParser);
		isMaxWidth = true;
		return true;
	}
	if (!stricmp("max-height", pAttrName))
	{
		settings.MaxHeight.ParseAttribute(pParser);
		isMaxHeight = true;
		return true;
	}

	return CIwManaged::ParseAttribute(pParser, pAttrName);
}

#endif