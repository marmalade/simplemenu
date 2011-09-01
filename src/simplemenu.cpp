#include <IwDebug.h>
#include <simplemenu.h>
#include <dpiInfo.h>
#include <freetypehelper.h>
#include "smMenu.h"
#include "smButton.h"
#include "smImage.h"
#include "smImageSource.h"
#include "smInput.h"
#include "smFloatingPanel.h"
#include "smTextBlock.h"
#include "smTextBox.h"
#include "smScriptProvider.h"
#include "smCanvas.h"
#include "smSlider.h"
#include "smGallery.h"
#include "smSelect.h"
#include "smCheckbox.h"
#include "smSwitch.h"
#include "smRow.h"
#include "smGrid.h"
#include "smFloatingPanel.h"
#include "smWindowHistory.h"

namespace SimpleMenu
{
	int initCounter = 0;
	CsmInputFilter* g_inputFilter = 0;
	iwangle g_toeLoadingAngle = 0;
	CIwArray<CsmScriptableClassDeclaration*>* toe_scriptClassDeclarations=0;

	CsmInputFilter* smGetInputFilter() 
	{
		if (!g_inputFilter)
		{
			g_inputFilter = new CsmInputFilter();
			g_inputFilter->Register();
		}
		return g_inputFilter;
	}

	smCloseState sm_menuCloseState = SM_KEEP_OPEN;
	smCloseState smGetCloseState() 
	{ 
		return sm_menuCloseState; 
	}
	class CsmUtils
	{
	public:
		static void CloseAllMenus() 
		{
			sm_menuCloseState = SM_CLOSE_ALL;
		}
		static void CloseMenu() 
		{
			sm_menuCloseState = SM_CLOSE_CURRENT;
		}
		static int GetDPI() {
			return DPI::dpiGetScreenDPI();
		}
		static int GetDeviceOS()
		{
			return s3eDeviceGetInt(S3E_DEVICE_OS);
		}
		static const char* GetDeviceOSName()
		{
			return s3eDeviceGetString(S3E_DEVICE_OS);
		}
		static int GetDeviceID()
		{
			return s3eDeviceGetInt(S3E_DEVICE_ID);
		}
		static const char* GetDeviceIDName()
		{
			return s3eDeviceGetString(S3E_DEVICE_ID);
		}
		static int HashString(const char* s)
		{
			return (int)IwHashString(s);
		}
		static void OpenMenuAtGroup(const char* s)
		{
			smOpenMenuAtGroup(s);
		}
		static void Alert(const char* s,const char* t)
		{
			smAlert(s,t);
		}
	};

	CsmScriptableClassDeclaration* smGetClassDescription()
	{
		static  TsmScriptableClassDeclaration<CsmUtils> d (0, "CsmUtils",
			ScriptTraits::Method("CloseMenu", &CsmUtils::CloseMenu),
			ScriptTraits::Method("CloseAllMenus", &CsmUtils::CloseAllMenus),
			ScriptTraits::Method("GetDPI", &CsmUtils::GetDPI),
			ScriptTraits::Method("GetDeviceOSName", &CsmUtils::GetDeviceOSName),
			ScriptTraits::Method("GetDeviceOS", &CsmUtils::GetDeviceOS),
			ScriptTraits::Method("GetDeviceIDName", &CsmUtils::GetDeviceIDName),
			ScriptTraits::Method("GetDeviceID", &CsmUtils::GetDeviceID),
			ScriptTraits::Method("HashString", &CsmUtils::HashString),
			ScriptTraits::Method("OpenMenuAtGroup", &CsmUtils::OpenMenuAtGroup),
			ScriptTraits::Method("Alert", &CsmUtils::Alert),
			
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
	 IW_CLASS_REGISTER(CsmCanvas);
	 IW_CLASS_REGISTER(CsmCheckbox);
	 IW_CLASS_REGISTER(CsmClickable);
	 IW_CLASS_REGISTER(CsmFloatingPanel);
	 IW_CLASS_REGISTER(CsmGallery);
	 IW_CLASS_REGISTER(CsmGrid);
	 IW_CLASS_REGISTER(CsmImage);
	 IW_CLASS_REGISTER(CsmImageTexture);
	 IW_CLASS_REGISTER(CsmItem);
	 IW_CLASS_REGISTER(CsmRow);
	 IW_CLASS_REGISTER(CsmSelect);
	 IW_CLASS_REGISTER(CsmSlider);
	 IW_CLASS_REGISTER(CsmStyleSheet);
	 IW_CLASS_REGISTER(CsmSwitch);
	 IW_CLASS_REGISTER(CsmTextBlock);
	 IW_CLASS_REGISTER(CsmTextBox);


	toe_scriptClassDeclarations = new CIwArray<CsmScriptableClassDeclaration*>;

	smRegisterClass(CsmMenu::GetClassDescription());
	smRegisterClass(CsmButton::GetClassDescription());
	smRegisterClass(CsmCanvas::GetClassDescription());
	smRegisterClass(CsmCheckbox::GetClassDescription());
	smRegisterClass(CsmClickable::GetClassDescription());
	smRegisterClass(CsmFloatingPanel::GetClassDescription());
	smRegisterClass(CsmGallery::GetClassDescription());
	smRegisterClass(CsmGrid::GetClassDescription());
	smRegisterClass(CsmImage::GetClassDescription());
	smRegisterClass(CsmImageSource::GetClassDescription());
	smRegisterClass(CsmItem::GetClassDescription());
	smRegisterClass(CsmRow::GetClassDescription());
	smRegisterClass(CsmSelect::GetClassDescription());
	smRegisterClass(CsmSlider::GetClassDescription());
	smRegisterClass(CsmSwitch::GetClassDescription());
	smRegisterClass(CsmTerminalItem::GetClassDescription());
	smRegisterClass(CsmTextBlock::GetClassDescription());
	smRegisterClass(CsmTextBox::GetClassDescription());
	smRegisterClass(smGetClassDescription());








	
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

	if (g_inputFilter)
	{
		g_inputFilter->UnRegister();
		delete g_inputFilter;
		g_inputFilter = 0;
	}

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

void SimpleMenu::smRenderLoading()
{
	IwGxClear(IW_GX_DEPTH_BUFFER_F);

	iwangle step = IW_GEOM_ONE/32;
	g_toeLoadingAngle = (g_toeLoadingAngle+step) %IW_GEOM_ONE;
	int32 h = (int32)IwGxGetScreenHeight();
	int32 w = (int32)IwGxGetScreenWidth();
	int32 x = w/2;
	int32 y = h/2;
	int r = (x<y)?(x/2):(y/2);
	int r2 = r*8/10;

	//iwfixed c = IwGeomCos(g_toeLoadingAngle);
	//iwfixed s = IwGeomCos(g_toeLoadingAngle);
	int32 vertices = 10*4;
	CIwSVec2* v = IW_GX_ALLOC(CIwSVec2,vertices);
	CIwColour* col = IW_GX_ALLOC(CIwColour,vertices);
	int index = 0;
	CIwColour c; c.Set(0xFFFFFFFF);
	col[index] = c;
	v[index++] = CIwSVec2(0,0);
	col[index] = c;
	v[index++] = CIwSVec2(0,h);
	col[index] = c;
	v[index++] = CIwSVec2(w,h);
	col[index] = c;
	v[index++] = CIwSVec2(w,0);
	iwangle a = g_toeLoadingAngle;
	int colS = 250/(vertices/4);
	for (int i=1; i<(vertices/4);++i)
	{
		iwfixed c1 = IwGeomCos(a);
		iwfixed s1 = IwGeomSin(a);
		a += step;
		iwfixed c2 = IwGeomCos(a);
		iwfixed s2 = IwGeomSin(a);
		c.r = c.g = c.b = 255-i*colS;
		col[index] = c;		v[index++] = CIwSVec2(x+c1*r/IW_GEOM_ONE,y+s1*r/IW_GEOM_ONE);
		col[index] = c;		v[index++] = CIwSVec2(x+c1*r2/IW_GEOM_ONE,y+s1*r2/IW_GEOM_ONE);
		c.r = c.g = c.b = 255-(i+1)*colS;
		col[index] = c;		v[index++] = CIwSVec2(x+c2*r2/IW_GEOM_ONE,y+s2*r2/IW_GEOM_ONE);
		col[index] = c;		v[index++] = CIwSVec2(x+c2*r/IW_GEOM_ONE,y+s2*r/IW_GEOM_ONE);

	}
	CIwMaterial* m = IW_GX_ALLOC_MATERIAL();
	m->SetColAmbient(255,255,255,255);
	IwGxClear(IW_GX_DEPTH_BUFFER_F);
	IwGxSetMaterial(m);
	IwGxLightingOff();
	IwGxSetVertStreamScreenSpace(v,vertices);
	IwGxSetUVStream(0);
	IwGxSetColStream(col);
	IwGxDrawPrims(IW_GX_QUAD_LIST, 0, index);
	IwGxFlush();
	IwGxSwapBuffers();
	IwGxClear(IW_GX_DEPTH_BUFFER_F);
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
