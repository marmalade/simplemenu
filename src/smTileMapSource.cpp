#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "smOSMTileMapSource.h"
#include "smMenu.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}


//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmTileMapSource::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmTileMapSource> d (0, "CsmTileMapSource",
			0);
	return &d;
}

//Constructor
CsmTileMapSource::CsmTileMapSource()
{
}

//Desctructor
CsmTileMapSource::~CsmTileMapSource()
{
}