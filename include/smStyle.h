#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include <fthFont.h>
#include <smBackground.h>

namespace SimpleMenu
{
	struct CsmStyleSettings
	{
		uint32 FontHash;
		FreeTypeHelper::CfthFont* Font;
		DPI::CdpiLength FontSize;
		CIwColour FontColor;
		CsmBackground Background;
		DPI::CdpiLength4 Margin;
		DPI::CdpiLength4 Padding;
		DPI::CdpiLength4 Border;
		DPI::CdpiLength MinWidth;
		DPI::CdpiLength MinHeight;
		DPI::CdpiLength MaxWidth;
		DPI::CdpiLength MaxHeight;
		CIwColour BorderColor;
		CIwColour ShadowColor;
		CIwColour TextShadowColor;
		DPI::CdpiLength ShadowSize;
		DPI::CdpiLength2 ShadowOffset;
		DPI::CdpiLength2 TextShadowOffset;
		bool DropShadow;
		iwfixed HorizontalAlignment;
		iwfixed VerticalAlignment;
		CsmStyleSettings():
			FontHash(0),Font(0),
			FontSize(8,DPI::CdpiLength::PT),
			HorizontalAlignment(0),VerticalAlignment(IW_GEOM_ONE/2),
			ShadowSize(3, DPI::CdpiLength::PT),
			DropShadow(false),
			MinWidth(0, DPI::CdpiLength::PX),
			MaxWidth(2147483647, DPI::CdpiLength::PX),
			MinHeight(0, DPI::CdpiLength::PX),
			MaxHeight(2147483647, DPI::CdpiLength::PX)
			{ FontColor.Set(0xFF000000); BorderColor.Set(0xFF000000); ShadowColor.Set(0x40000000); TextShadowColor.Set(0x40000000);}
		void Serialise();
		void Inherit(CsmStyleSettings* other)
		{
			if (!other)
				return;
			FontHash = other->FontHash;
			Font = other->Font;
			FontSize = other->FontSize;
			FontColor = other->FontColor;
			BorderColor = other->BorderColor;
			ShadowSize = other->ShadowSize;
			ShadowOffset = other->ShadowOffset;
			TextShadowOffset = other->TextShadowOffset;
			TextShadowColor = other->TextShadowColor;
			HorizontalAlignment = other->HorizontalAlignment;
			VerticalAlignment = other->VerticalAlignment;
		}
		inline int16 GetMarginLeft(int16 total)const {return (int16)Margin.left.GetPx(total);}
		inline int16 GetMarginTop(int16 total)const {return (int16)Margin.top.GetPx(total);}
		inline int16 GetMarginRight(int16 total)const {return (int16)Margin.right.GetPx(total);}
		inline int16 GetMarginBottom(int16 total)const {return (int16)Margin.bottom.GetPx(total);}

		inline int16 GetBorderLeft(int16 total)const {return (int16)Border.left.GetPx(total);}
		inline int16 GetBorderTop(int16 total)const {return (int16)Border.top.GetPx(total);}
		inline int16 GetBorderRight(int16 total)const {return (int16)Border.right.GetPx(total);}
		inline int16 GetBorderBottom(int16 total)const {return (int16)Border.bottom.GetPx(total);}

		inline int16 GetPaddingLeft(int16 total)const {return (int16)Padding.left.GetPx(total);}
		inline int16 GetPaddingTop(int16 total)const {return (int16)Padding.top.GetPx(total);}
		inline int16 GetPaddingRight(int16 total)const {return (int16)Padding.right.GetPx(total);}
		inline int16 GetPaddingBottom(int16 total)const {return (int16)Padding.bottom.GetPx(total);}

		bool operator == (const CsmStyleSettings & other)const
		{
			return (Font == other.Font)
				&& (FontSize == other.FontSize)
				&& (FontColor == other.FontColor)
				&& (Background == other.Background)
				&& (Margin == other.Margin)
				&& (Padding == other.Padding)
				&& (Border == other.Border)
				&& (ShadowColor == other.ShadowColor)
				&& (TextShadowColor == other.TextShadowColor)
				&& (ShadowOffset == other.ShadowOffset)
				&& (TextShadowOffset == other.TextShadowOffset)
				&& (ShadowColor == other.ShadowColor)
				&& (DropShadow == other.DropShadow)
				&& (BorderColor == other.BorderColor)
				&& (VerticalAlignment == other.VerticalAlignment)
				&& (HorizontalAlignment == other.HorizontalAlignment);
		}
	};

	class CsmStyle: public CIwManaged
	{
	public:
		CsmStyleSettings settings;
	protected:
		bool isFont;
		bool isFontSize;
		bool isFontColor;
		bool isBackground;
		bool isMargin;
		bool isPadding;
		bool isBorder;
		bool isShadowSize;
		bool isShadowOffset;
		bool isTextShadowOffset;
		bool isShadowColor;
		bool isTextShadowColor;
		bool isDropShadow;
		bool isBorderColor;
		bool isHorizontalAlignment;
		bool isVerticalAlignment;
		bool isMinWidth;
		bool isMinHeight;
		bool isMaxWidth;
		bool isMaxHeight;
	public:
		CsmStyle():isFont(false),isFontSize(false),isFontColor(false),isBackground(false),isMargin(false),isPadding(false),
			isShadowSize(false),isShadowOffset(false),isTextShadowOffset(false),isShadowColor(false),isTextShadowColor(false),isDropShadow(false),
			isBorder(false),isBorderColor(false),isHorizontalAlignment(false),isVerticalAlignment(false),
			isMinWidth(false),
			isMinHeight(false),
			isMaxWidth(false),
			isMaxHeight(false){}
		virtual void Serialise();
		void Apply(CsmStyleSettings* other)
		{
			if (isFont)
			{
				other->FontHash = settings.FontHash;
				other->Font = settings.Font;
			}
			if (isFontSize)
				other->FontSize = settings.FontSize;
			if (isFontColor)
				other->FontColor = settings.FontColor;
			if (isBackground)
				other->Background = settings.Background;
			if (isMargin)
				other->Margin = settings.Margin;
			if (isBorderColor)
				other->BorderColor = settings.BorderColor;
			if (isBorder)
				other->Border = settings.Border;
			if (isDropShadow)
				other->DropShadow = settings.DropShadow;
			if (isShadowColor)
				other->ShadowColor = settings.ShadowColor;
			if (isTextShadowColor)
				other->TextShadowColor = settings.TextShadowColor;
			if (isShadowOffset)
				other->ShadowOffset = settings.ShadowOffset;
			if (isTextShadowOffset)
				other->TextShadowOffset = settings.TextShadowOffset;
			if (isShadowSize)
				other->ShadowSize = settings.ShadowSize;
			if (isPadding)
				other->Padding = settings.Padding;
			if (isHorizontalAlignment)
				other->HorizontalAlignment = settings.HorizontalAlignment;
			if (isVerticalAlignment)
				other->VerticalAlignment = settings.VerticalAlignment;
			if (isMinWidth)
				other->MinWidth = settings.MinWidth;
			if (isMinHeight)
				other->MinHeight = settings.MinHeight;
			if (isMaxWidth)
				other->MaxWidth = settings.MaxWidth;
			if (isMaxHeight)
				other->MaxHeight = settings.MaxHeight;
		}

#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}