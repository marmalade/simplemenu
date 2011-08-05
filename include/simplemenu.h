#pragma once
#include <string>
#include <IwTextParserITX.h>
#include <iwgeom.h>
namespace SimpleMenu
{
	void smInit();
	void smTerminate();

	void smSerialiseString(std::string & s);	
	void smReadString(CIwTextParserITX* pParser, std::string* s);
	void smDrawSimpleMenuScrollbar(const CIwSVec2 & pos,const CIwSVec2 & size,const CIwSVec2 & spos,const CIwSVec2 & ssize);
	void smRenderLoading();

	enum smCloseState
	{
		SM_KEEP_OPEN = 0,
		SM_CLOSE_CURRENT = 0,
		SM_CLOSE_ALL = 1
	};

	smCloseState smGetCloseState();
}
