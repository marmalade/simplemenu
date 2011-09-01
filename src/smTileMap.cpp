#include <IwDebug.h>
#include <simplemenu.h>
#include <smTileMap.h>
#include <smTileMapImageSource.h>
#include <smTileMapSource.h>

namespace SimpleMenu
{
	int initTileMapCounter = 0;
}

using namespace SimpleMenu;

void SimpleMenu::smTileMapInit()
{
	++initTileMapCounter;
	if (initTileMapCounter != 1)
		return;

	smInit();

	smRegisterClass(CsmTileMapImageSource::GetClassDescription());
	smRegisterClass(CsmTileMapSource::GetClassDescription());
	IW_CLASS_REGISTER(CsmTileMapImageSource);
}

void SimpleMenu::smTileMapTerminate()
{
	--initTileMapCounter;
	if (initTileMapCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smTileMapTerminate doesn't match smTileMapInit"));
	if (initTileMapCounter != 0)
		return;

	smTerminate();
}