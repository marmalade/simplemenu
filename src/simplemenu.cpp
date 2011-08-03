#include <IwDebug.h>
#include <simplemenu.h>
#include <dpiInfo.h>
#include <freetypehelper.h>
#include "smMenu.h"
#include "smButton.h"
#include "smImage.h"
#include "smInput.h"
#include "smTextBlock.h"
#include "smTextBox.h"
#include "smScriptProvider.h"
#include "smCanvas.h"
#include "smSlider.h"
#include "smGallery.h"

namespace SimpleMenu
{
	int initCounter = 0;
	CsmInputFilter* g_inputFilter = 0;
	CIwArray<CsmScriptableClassDeclaration*>* toe_scriptClassDeclarations=0;

	CsmInputFilter* smGetInputFilter() {return g_inputFilter;}

	smCloseState sm_menuCloseState = SM_KEEP_OPEN;
	smCloseState smGetCloseState() { return sm_menuCloseState; }

	class CsmUtils
	{
	public:
		static void CloseAllMenus() {sm_menuCloseState= SM_CLOSE_ALL;}
		static void CloseMenu() {sm_menuCloseState= SM_CLOSE_CURRENT;}
		static int GetDPI() {
			return DPI::dpiGetScreenDPI();
		}
	};

	CsmScriptableClassDeclaration* smGetClassDescription()
	{
		static  TsmScriptableClassDeclaration<CsmUtils> d ("CsmUtils",
			ScriptTraits::Method("CloseMenu", &CsmUtils::CloseMenu),
			ScriptTraits::Method("CloseAllMenus", &CsmUtils::CloseAllMenus),
			ScriptTraits::Method("GetDPI", &CsmUtils::GetDPI),
				0);
		return &d;
	}


}

using namespace SimpleMenu;

void SimpleMenu::smInit()
{
	++initCounter;
	if (initCounter != 1)
		return;

	DPI::dpiInit();
	FreeTypeHelper::fthInit();

	#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CsmMenuResHandler);
	IwGetResManager()->AddHandler(new CsmStyleSheetResHandler);
	#endif

	IW_CLASS_REGISTER(CsmMenu);
	IW_CLASS_REGISTER(CsmBackground);
	IW_CLASS_REGISTER(CsmButton);
	IW_CLASS_REGISTER(CsmClickable);
	IW_CLASS_REGISTER(CsmImage);
	IW_CLASS_REGISTER(CsmSlider);
	IW_CLASS_REGISTER(CsmCanvas);
	IW_CLASS_REGISTER(CsmGallery);
	IW_CLASS_REGISTER(CsmItem);
	IW_CLASS_REGISTER(CsmTextBlock);
	IW_CLASS_REGISTER(CsmStyleSheet);
	//IW_CLASS_REGISTER(CsmSlider);
	//IW_CLASS_REGISTER(CsmRow);
	//IW_CLASS_REGISTER(CsmCompass);
	//IW_CLASS_REGISTER(CsmGrid);


	toe_scriptClassDeclarations = new CIwArray<CsmScriptableClassDeclaration*>;
	smRegisterClass(CsmMenu::GetClassDescription());
	smRegisterClass(CsmButton::GetClassDescription());
	smRegisterClass(CsmClickable::GetClassDescription());
	smRegisterClass(CsmImage::GetClassDescription());
	smRegisterClass(CsmItem::GetClassDescription());
	smRegisterClass(CsmTextBlock::GetClassDescription());
	smRegisterClass(smGetClassDescription());

	g_inputFilter = new CsmInputFilter();
}

void SimpleMenu::smTerminate()
{
	--initCounter;
	if (initCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smTerminate doesn't match smInit"));
	if (initCounter != 0)
		return;

	FreeTypeHelper::fthTerminate();
	DPI::dpiTerminate();

	delete g_inputFilter;
	g_inputFilter = 0;

	if (toe_scriptClassDeclarations)
	{
		for (CIwArray<CsmScriptableClassDeclaration*>::iterator i = toe_scriptClassDeclarations->begin(); i!=toe_scriptClassDeclarations->end(); ++i)
		{
			(*i)->Close();
			//delete(*i);
		}
		delete toe_scriptClassDeclarations;
		toe_scriptClassDeclarations = 0;
	}
}

void SimpleMenu::smRegisterClass(CsmScriptableClassDeclaration* c)
{
	toe_scriptClassDeclarations->push_back(c);
}

void SimpleMenu::smRegisterScriptableClasses(IsmScriptProvider* system)
{
	if (!toe_scriptClassDeclarations)
		return;
	for (CIwArray<CsmScriptableClassDeclaration*>::iterator i = toe_scriptClassDeclarations->begin(); i!=toe_scriptClassDeclarations->end(); ++i)
	{
		system->RegisterClass(*i);
	}
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
