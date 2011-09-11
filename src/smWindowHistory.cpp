#include "simplemenu.h"
#include "smWindowHistory.h"
//#include "smButton.h"
//#include "smRow.h"
//
//using namespace SimpleMenu;
//
//namespace SimpleMenu
//{
//	extern smCloseState sm_menuCloseState;
//
//	CsmMenu* g_currentMenu = 0;
//}
//
//void SimpleMenu::smOpenMenuAtGroup(const char* s)
//{
//	CIwResGroup* group = IwGetResManager()->LoadGroup(s);
//	if (!group)
//		return;
//	CIwResList* list = group->GetListNamed("CsmMenu", IW_RES_PERMIT_NULL_F | IW_RES_IGNORE_SHARED_F | IW_RES_IGNORE_CHILDREN_F);
//	if (list)
//	{
//		if (list->m_Resources.GetSize() > 0)
//		{
//			CsmMenu* m = (CsmMenu*)list->m_Resources[0];
//			smShowMenu(m, smGetInputFilter(), smGetDefaultScriptProvider(), 0, 0);
//		}
//	}
//	IwGetResManager()->DestroyGroup(group);
//}
//
//void SimpleMenu::smShowMenu(CsmMenu* m, CsmInputFilter* input, IsmScriptProvider* script, smUpdateCallback callback, void* context)
//{
//	if (!m)
//		return;
//
//	CsmInputQueue* inputQueue = input->PushQueue();
//	CsmMenu* prevMenu = g_currentMenu;
//	g_currentMenu = m;
//
//	inputQueue->RegisterReceiver(m);
//	m->Initialize(script);
//
//	clock_t cur,prev;
//	cur = prev = clock();
//
//	m->SetTransition(-IW_GEOM_ONE);
//
//	for (;;)
//	{
//		cur = clock();
//		clock_t ms = (1000/30)-(cur-prev);
//		if (ms < 0) ms = 0;
//		s3eDeviceYield(ms);
//		cur = clock();
//		ms = (cur-prev); if (ms > 1000) ms = 1000; //Limit to 1 FPS
//		prev = cur;
//		
//		s3eKeyboardUpdate();
//
//		bool result = true;
//
//		if (callback)
//			if (!((*callback)(m, context)))
//				result = false;
//
//		if (result)
//			m->Update(IW_GEOM_ONE*ms/1000);
//
//		if (s3eDeviceCheckQuitRequest())
//			sm_menuCloseState = SM_CLOSE_ALL;
//		if	(SimpleMenu::smGetCloseState() != SimpleMenu::SM_KEEP_OPEN)
//			result = false;
//		if ((s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN) ||
//			(s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN))
//			result = false;
//
//		if (!result && SimpleMenu::smGetCloseState() == SimpleMenu::SM_KEEP_OPEN)
//		{
//			sm_menuCloseState = SM_CLOSE_CURRENT;
//		}
//
//		iwfixed prevT = m->GetTransition();
//		if (prevT < 0)
//		{
//			prevT += IW_GEOM_ONE*ms/300;
//			if (prevT > 0) prevT = 0;
//			m->SetTransition(prevT);
//		} 
//		else if (!result)
//		{
//			if (SimpleMenu::smGetCloseState() == SimpleMenu::SM_CLOSE_ALL) break;
//			prevT += IW_GEOM_ONE*ms/300;
//			if (prevT > IW_GEOM_ONE) break;
//			m->SetTransition(prevT);
//		}
//
//		m->Prepare();
//		IwGxClear(IW_GX_DEPTH_BUFFER_F);
//		//IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
//
//		m->Render();
//
//		IwGxFlush();
//		IwGxSwapBuffers();
//	}
//
//	if (SimpleMenu::smGetCloseState() == SimpleMenu::SM_CLOSE_CURRENT)
//	{
//		sm_menuCloseState = SM_KEEP_OPEN;
//	} else if (SimpleMenu::smGetCloseState() == SimpleMenu::SM_SWITCH_CURRENT)
//
//	inputQueue->UnRegisterReceiver(m);
//	input->PopQueue(inputQueue);
//	g_currentMenu = prevMenu;
//	IwGxClear(IW_GX_DEPTH_BUFFER_F);
//}
//void SimpleMenu::smShowMenu(CsmMenu* m, smUpdateCallback callback, void* context)
//{
//	CsmInputFilter* input = 0;
//	IsmScriptProvider* script = 0;
//	if (g_currentMenu)
//	{
//		script = g_currentMenu->GetScriptProvider();
//	}
//	if (!input) input = smGetInputFilter();
//	if (!script) script = smGetDefaultScriptProvider();
//	smShowMenu(m,input, script, callback, context);
//}
//CsmMenu* SimpleMenu::smCreateDialogWindow(const char* title, const char* text)
//{
//	CsmMenu* menu = new CsmMenu();
//	if (g_currentMenu)
//	{
//		menu->SetStyleSheetHash(g_currentMenu->GetStyleSheetHash());
//	}
//	CsmItem* content = new CsmItem(); content->SetStyleClass("Content"); menu->AddItem(content);
//	CsmItem* header = new CsmItem(); header->SetStyleClass("Header"); menu->AddItem(header);
//	CsmRow* footer = new CsmRow(); footer->SetStyleClass("Footer"); menu->AddItem(footer);
//	if (title)
//		header->AddTextBlock(title);
//	if (text)
//		content->AddTextBlock(text);
//	CsmButton* b;
//	b = new CsmButton(); footer->AddItem(b);
//	b->AddTextBlock("Cancel");
//	b->SetOnClick("CsmUtils.CloseMenu()");
//	return menu;
//}
//void SimpleMenu::smOpenWaitDialog(const char* title, const char* text, smUpdateCallback callback, void* context)
//{
//	CsmMenu* menu = smCreateDialogWindow(title, text);
//	smShowMenu(menu, callback, context);
//	delete menu;
//}
//void SimpleMenu::smAlert(const char* title, const char* text)
//{
//	CsmMenu* menu = smCreateDialogWindow(title, text);
//	smShowMenu(menu, 0, 0);
//	delete menu;
//}
//
void SimpleMenu::smAssertMessage(const char* title,const char* message,bool v)
{
	if (!v)
		smAlert(title, message);
}