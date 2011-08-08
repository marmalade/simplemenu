#include <IwAnim.h>
#include "sm3D.h"
#include "simplemenu.h"
#include <pugixml.hpp>
#include "sm3DModel.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	int init3DCounter = 0;
}

void SimpleMenu::sm3DInit()
{
	++init3DCounter;
	if (init3DCounter != 1)
		return;

	IwAnimInit();
	smInit();

	IW_CLASS_REGISTER(Csm3DModel);
	smRegisterClass(Csm3DModel::GetClassDescription());
}

void SimpleMenu::sm3DTerminate()
{
	--init3DCounter;
	if (init3DCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("sm3DTerminate doesn't match sm3DInit"));
	if (init3DCounter != 0)
		return;

	smTerminate();
	IwAnimTerminate();
}