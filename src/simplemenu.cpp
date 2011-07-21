#include <IwDebug.h>
#include <simplemenu.h>
#include <dpiInfo.h>
#include <freetypehelper.h>
#include "smMenu.h"

namespace SimpleMenu
{
	int initCounter = 0;
}

using namespace SimpleMenu;

void SimpleMenu::smInit()
{
	++initCounter;
	if (initCounter != 1)
		return;

	#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CsmMenuResHandler);
	IwGetResManager()->AddHandler(new CsmStyleSheetResHandler);
	#endif

	IW_CLASS_REGISTER(CsmMenu);
	IW_CLASS_REGISTER(CsmBackground);
	//IW_CLASS_REGISTER(CsmClickable);
	//IW_CLASS_REGISTER(CsmCanvas);
	//IW_CLASS_REGISTER(CsmMenu);
	//IW_CLASS_REGISTER(CsmImage);
	//IW_CLASS_REGISTER(CsmImageCarousel);
	//IW_CLASS_REGISTER(CsmInputComponent);
	IW_CLASS_REGISTER(CsmItem);
	IW_CLASS_REGISTER(CsmTextBlock);
	IW_CLASS_REGISTER(CsmStyleSheet);
	//IW_CLASS_REGISTER(CsmSlider);
	//IW_CLASS_REGISTER(CsmButton);
	//IW_CLASS_REGISTER(CsmRow);
	//IW_CLASS_REGISTER(CsmCompass);
	//IW_CLASS_REGISTER(CsmGrid);

	DPI::dpiInit();
	FreeTypeHelper::fthInit();

	//IW_CLASS_REGISTER(CfthFont);
}

void SimpleMenu::smTerminate()
{
	--initCounter;
	if (initCounter < 0)
		IwAssertMsg(FREETYPE,false,("smTerminate doesn't match smInit"));
	if (initCounter != 0)
		return;

	FreeTypeHelper::fthTerminate();
	DPI::dpiTerminate();
}
void	SimpleMenu::smReadString(CIwTextParserITX* pParser, std::string* s)
{
		char* c = pParser->ReadString();
		*s = c;
		delete c;
}

void SimpleMenu::smDrawSimpleMenuScrollbar(const CIwSVec2 & pos,const CIwSVec2 & size,const CIwSVec2 & spos,const CIwSVec2 & ssize)
{
	CIwMaterial* m = IW_GX_ALLOC_MATERIAL();
	CIwColour c;
	c.Set(0xFFFFFFFF);
	m->SetColAmbient(c);
	m->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(m);
	CIwSVec2* points = IW_GX_ALLOC(CIwSVec2,8);
	CIwColour* cols = IW_GX_ALLOC(CIwColour,8);
	points[0] = pos+CIwSVec2(0,0);
	points[1] = pos+CIwSVec2(0,size.y);
	points[2] = pos+CIwSVec2(size.x,size.y);
	points[3] = pos+CIwSVec2(size.x,0);
	c.Set(0x20,0x20,0x20,0x20);
	cols[0] = cols[1] = cols[2] = cols[3] = c;
	points[4] = spos+CIwSVec2(0,0);
	points[5] = spos+CIwSVec2(0,ssize.y);
	points[6] = spos+CIwSVec2(ssize.x,ssize.y);
	points[7] = spos+CIwSVec2(ssize.x,0);
	c.Set(0xFF,0xFF,0xFF,0x80);
	cols[4] = cols[5] = cols[6] = cols[7] = c;
	IwGxSetVertStreamScreenSpace(points,8);
	IwGxSetColStream(cols);
	IwGxDrawPrims(IW_GX_QUAD_LIST,0,8);
}

//Reads/writes a binary file using @a IwSerialise interface.
void SimpleMenu::smSerialiseString (std::string & s)
{
	if (IwSerialiseIsWriting())
	{
		uint32 len = s.size();
		IwSerialiseUInt32(len);
		for (std::string::iterator i=s.begin(); i!=s.end();++i)
		{
			IwSerialiseChar(*i);
		}
	}
	else
	{
		uint32 len;
		IwSerialiseUInt32(len);
		if (!len)
		{
			s.clear();
		}
		else
		{
			s.resize(len,' ');
			for (std::string::iterator i=s.begin(); i!=s.end() && len;++i,--len)
			{
				IwSerialiseChar(*i);
			}
		}
	}
}
