#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smOSMTileMapSource.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmOSMTileMapSource);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmOSMTileMapSource);

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmOSMTileMapSource::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmOSMTileMapSource> d (CsmTileMapSource::GetClassDescription(), "CsmOSMTileMapSource",
			0);
	return &d;
}

//Constructor
CsmOSMTileMapSource::CsmOSMTileMapSource()
{
}

//Desctructor
CsmOSMTileMapSource::~CsmOSMTileMapSource()
{
}