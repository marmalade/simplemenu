#include "simplemenu.h"
#include "smWindowHistory.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	extern smCloseState sm_menuCloseState;

	CsmMenu* g_currentMenu = 0;
}

void SimpleMenu::smOpenMenuAtGroup(const char* s)
{
	CIwResGroup* group = IwGetResManager()->LoadGroup(s);
	if (!group)
		return;
	CIwResList* list = group->GetListNamed("CsmMenu", IW_RES_PERMIT_NULL_F | IW_RES_IGNORE_SHARED_F | IW_RES_IGNORE_CHILDREN_F);
	if (list)
	{
		if (list->m_Resources.GetSize() > 0)
		{
			CsmMenu* m = (CsmMenu*)list->m_Resources[0];
			smShowMenu(m, smGetInputFilter(), smGetDefaultScriptProvider(), 0, 0);
		}
	}
	IwGetResManager()->DestroyGroup(group);
}

void SimpleMenu::smShowMenu(CsmMenu* m, CsmInputFilter* input, IsmScriptProvider* script, smUpdateCallback callback, void* context)
{
	if (!m)
		return;

	input->RegisterReceiver(m);
	m->Initialize(script);
	CsmMenu* prevMenu = g_currentMenu;
	g_currentMenu = m;

	clock_t cur,prev;
	cur = prev = clock();

	for (;;)
	{
		cur = clock();
		clock_t ms = (1000/30)-(cur-prev);
		if (ms < 0) ms = 0;
		prev = cur;
		s3eDeviceYield(ms);

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
		m->Update(IW_GEOM_ONE/30);
		m->Render();

		IwGxFlush();
		IwGxSwapBuffers();
	}

	if (SimpleMenu::smGetCloseState() == SimpleMenu::SM_CLOSE_CURRENT)
	{
		sm_menuCloseState = SM_KEEP_OPEN;
	}

	input->UnRegisterReceiver(m);
	g_currentMenu = prevMenu;
	IwGxClear(IW_GX_DEPTH_BUFFER_F);
}