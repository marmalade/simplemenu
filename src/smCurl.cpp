#include <simplemenu.h>
#include "smCurl.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	int initCurlCounter = 0;
}

void SimpleMenu::smCurlInit()
{
	++initCurlCounter;
	if (initCurlCounter != 1)
		return;

	smInit();

	//IW_CLASS_REGISTER(CsmCurlModel);
	//smRegisterClass(CsmCurlModel::GetClassDescription());
}

void SimpleMenu::smCurlTerminate()
{
	--initCurlCounter;
	if (initCurlCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smCurlTerminate doesn't match smCurlInit"));
	if (initCurlCounter != 0)
		return;

	smTerminate();
}