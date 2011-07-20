#include <IwDebug.h>
#include <simplemenu.h>
#include <dpiInfo.h>
#include <freetypehelper.h>

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