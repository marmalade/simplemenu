#pragma once

#include <pugixml.hpp>
#include <smCurlRequest.h>

namespace SimpleMenu
{
	class CsmOsmXApiRequest
	{
	protected:
		CsmCurlMemoryRequest request;
	public:
		//Constructor
		CsmOsmXApiRequest();
		//Desctructor
		~CsmOsmXApiRequest();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
	};
}