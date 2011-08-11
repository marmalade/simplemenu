#include <s3e.h>
#include <IwGx.h>
#include <IwGraphics.h>
#include <freetypehelper.h>
#include <fthFont.h>
#include <simplemenu.h>
#include <smMenu.h>
#include <smInput.h>
#include <smLuaState.h>
#include <smAllFeatures.h>
#include <smConfig.h>
#include <sm3DModel.h>
#include <smCurl.h>
#include <smOsm.h>
#include <smLuaState.h>
#include <smWindowHistory.h>

using namespace FreeTypeHelper;

//-----------------------------------------------------------------------------
// Main global function
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	IwGxInit();
	IwResManagerInit();
	IwGraphicsInit();
	fthInit();
	SimpleMenu::smInit();
	SimpleMenu::smLuaInit();
	SimpleMenu::smFeaturesInit();
	SimpleMenu::sm3DInit();
	SimpleMenu::smCurlInit();
	SimpleMenu::smOsmInit();
	SimpleMenu::smSetLuaAsDefaultScript();
	IwGxSetColClear(0x1f, 0x1f, 0xc0, 0x7f);

	SimpleMenu::CsmInputFilter* input = SimpleMenu::smGetInputFilter();

	SimpleMenu::CsmLuaState* lua = SimpleMenu::smGetLuaDefaultState();

	CIwResGroup* sampleGroup =  IwGetResManager()->LoadGroup("menu/mainmenu.group");
	SimpleMenu::CsmMenu* m = (SimpleMenu::CsmMenu*)sampleGroup->GetResNamed("mainmenu", "CsmMenu");

	SimpleMenu::smShowMenu(m, input, lua, 0, 0);

	IwGetResManager()->DestroyGroup(sampleGroup);

	SimpleMenu::smOsmTerminate();
	SimpleMenu::smCurlTerminate();
	SimpleMenu::sm3DTerminate();
	SimpleMenu::smFeaturesTerminate();
	SimpleMenu::smLuaTerminate();
	SimpleMenu::smTerminate();
	fthTerminate();
	IwGraphicsTerminate();
	IwResManagerTerminate();
	IwGxTerminate();
}