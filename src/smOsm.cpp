#include <smCurl.h>
#include "smOsm.h"
#include "smOsmXApiRequest.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	int initOsmCounter = 0;
}


//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmOsm::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmOsm> d ("CsmOsm",
			//ScriptTraits::Method("GetRoot", &CsmCurlImageSource::GetRoot),
			0);
	return &d;
}

void SimpleMenu::smOsmInit()
{
	++initOsmCounter;
	if (initOsmCounter != 1)
		return;

	smCurlInit();

	smRegisterClass(CsmOsmXApiRequest::GetClassDescription());
	smRegisterClass(CsmOsm::GetClassDescription());
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

	//double z = (1 << maxZoom);
	//double node_x = (( (float)((lon + 180.0) / 360.0 * z) )) ;
	//double node_y = (( (float)((1.0 - log(tan(lat * M_PI / 180.0) + 1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0 * z) ));
	//longitude = (int32)(256*node_x);
	//latitude = (int32)(256*node_y);
