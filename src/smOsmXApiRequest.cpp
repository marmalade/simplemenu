#include "smOsmXApiRequest.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

CsmOsmXApiRequest::CsmOsmXApiRequest()
{
}
CsmOsmXApiRequest::~CsmOsmXApiRequest()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmOsmXApiRequest::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmOsmXApiRequest> d ("CsmOsmXApiRequest",
			//ScriptTraits::Method("GetRoot", &CsmCurlImageSource::GetRoot),
			0);
	return &d;
}