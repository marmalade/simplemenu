#include <simplemenu.h>
#include <curl/curl.h>
#include "smCurl.h"
#include "smCurlRequest.h"

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

	//IW_CLASS_REGISTER(CsmCurlRequest);
	smRegisterClass(CsmCurlRequest::GetClassDescription());

	curl_global_init(0);
}

void SimpleMenu::smCurlTerminate()
{
	--initCurlCounter;
	if (initCurlCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smCurlTerminate doesn't match smCurlInit"));
	if (initCurlCounter != 0)
		return;
	curl_global_cleanup();
	smTerminate();
}