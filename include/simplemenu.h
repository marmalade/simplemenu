#pragma once
#include <string>
#include <IwTextParserITX.h>
#include <iwgeom.h>

#define smAssert(a) (SimpleMenu::smAssertMessage("Error!", #a, smAssert))

namespace SimpleMenu
{
	void smInit();

	void smTerminate();

	void smSerialiseString(std::string & s);	

	template <class T> void smSerialiseManaged(T* & var)
	{
		if (IwSerialiseIsReading ())
		{
			CIwManaged* m; IwSerialiseManagedObject(m); if (m) { var = static_cast<T*>(m); } else { var = 0; }
		}
		else
		{
			CIwManaged* m = static_cast<CIwManaged*>(var); IwSerialiseManagedObject(m);
		}
	}

	void smReadString(CIwTextParserITX* pParser, std::string* s);

	void smDrawSimpleMenuScrollbar(const CIwSVec2 & pos,const CIwSVec2 & size,const CIwSVec2 & spos,const CIwSVec2 & ssize);

	void smRequestAutoRenderLoading();
	void smReleaseAutoRenderLoading();
	void smRenderLoading();

	void smAssertMessage(const char* title,const char* message,bool v);

	enum smCloseState
	{
		SM_KEEP_OPEN = 0,
		SM_CLOSE_CURRENT = 1,
		SM_CLOSE_ALL = 2,
		SM_SWITCH_CURRENT = 3
	};

	smCloseState smGetCloseState();


}
