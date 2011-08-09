#include "simplemenu.h"
#include "smWindowHistory.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	extern smCloseState sm_menuCloseState;

	CsmMenu* g_currentMenu = 0;
}

void SimpleMenu::smShowMenu(CsmMenu* m, CsmInputFilter* input, IsmScriptProvider* script, smUpdateCallback callback, void* context)
{
	if (!m)
		return;

	input->RegisterReceiver(m);
	m->Initialize(script);
	CsmMenu* prevMenu = g_currentMenu;
	g_currentMenu = m;

	for (;;)
	{
		s3eDeviceYield(0);
		s3eKeyboardUpdate();

		bool result = true;

		if (callback)
			if (!((*callback)(m, context)))
				result = false;

		if	(
			(result == false) ||
			(s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN) ||
			(s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN) ||
			(s3eDeviceCheckQuitRequest()) ||
			(SimpleMenu::smGetCloseState() != SimpleMenu::SM_KEEP_OPEN)
			)
			break;


		IwGxClear(IW_GX_DEPTH_BUFFER_F);
		//IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
		m->Update(30);
		m->Render();

		IwGxFlush();
		IwGxSwapBuffers();
	}

	if (SimpleMenu::smGetCloseState() != SimpleMenu::SM_CLOSE_CURRENT)
	{
		sm_menuCloseState = SM_KEEP_OPEN;
	}

	input->UnRegisterReceiver(m);
	g_currentMenu = prevMenu;
}