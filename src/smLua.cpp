#include <IwDebug.h>
#include <smLua.h>
#include <smLuaScript.h>
#include <smLuaState.h>

namespace SimpleMenu
{
	int initLuaCounter = 0;
}

using namespace SimpleMenu;

void SimpleMenu::smLuaInit()
{
	++initLuaCounter;
	if (initLuaCounter != 1)
		return;

	#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CsmLuaScriptResHandler);
	#endif

	IW_CLASS_REGISTER(CsmLuaState);
	IW_CLASS_REGISTER(CsmLuaScript);

	smInit();
}

void SimpleMenu::smLuaTerminate()
{
	--initLuaCounter;
	if (initLuaCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smLuaTerminate doesn't match smLuaInit"));
	if (initLuaCounter != 0)
		return;

	smTerminate();
}