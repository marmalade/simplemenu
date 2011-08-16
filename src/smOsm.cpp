#include <math.h>
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
	static  TsmScriptableClassDeclaration<CsmOsm> d (0, "CsmOsm",
			ScriptTraits::Method("GetDistance", &CsmOsm::GetDistance),
			0);
	return &d;
}

float CsmOsm::GetDistance(float lon1, float lat1, float lon2, float lat2)
{
	float rho = 3960.0f;
    float phi_1 = (90.0f - lat1) * 3.1415926535897932384626433832795f / 180.0f;
    float phi_2 = (90.0f - lat2) * 3.1415926535897932384626433832795f / 180.0f;
    float theta_1 = lon1 * 3.1415926535897932384626433832795f / 180.0f;
    float theta_2 = lon2 * 3.1415926535897932384626433832795f / 180.0f;
    float d = rho * acosf(
       sinf(phi_1) *
       sinf(phi_2) *
       cosf(theta_1 - theta_2) +
       cosf(phi_1) *
       cosf(phi_2)
     );
     float output = 1609.344*d;
     return output;
}

void SimpleMenu::smOsmInit()
{
	++initOsmCounter;
	if (initOsmCounter != 1)
		return;

	smCurlInit();

	smRegisterClass(CsmOsmXApiRequest::GetClassDescription());
	smRegisterClass(CsmOsm::GetClassDescription());
	smRegisterClass(CsmOsmNode::GetClassDescription());
	smRegisterClass(CsmOsmWay::GetClassDescription());
	smRegisterClass(CsmOsmTag::GetClassDescription());
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
