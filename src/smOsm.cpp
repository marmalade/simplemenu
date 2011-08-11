#include <smCurl.h>
#include "smOsm.h"
#include "smOsmXApiRequest.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	int initOsmCounter = 0;
}

void SimpleMenu::smOsmInit()
{
	++initOsmCounter;
	if (initOsmCounter != 1)
		return;

	smCurlInit();

	smRegisterClass(CsmOsmXApiRequest::GetClassDescription());

}

void SimpleMenu::smOsmTerminate()
{
	--initOsmCounter;
	if (initOsmCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smOsmTerminate doesn't match smOsmInit"));
	if (initOsmCounter != 0)
		return;

	smCurlTerminate();
}