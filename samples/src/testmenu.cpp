#include <s3e.h>
#include <IwGx.h>
#include <IwGraphics.h>
#include <freetypehelper.h>
#include <fthFont.h>
#include <simplemenu.h>
#include <smMenu.h>
#include <smInput.h>
#include <smLuaState.h>

using namespace FreeTypeHelper;

//-----------------------------------------------------------------------------
// Main global function
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	IwGxInit();
	IwResManagerInit();
	IwGraphicsInit();
	fthInit();
	SimpleMenu::smInit();

	IwGxSetColClear(0x1f, 0x1f, 0xc0, 0x7f);

	SimpleMenu::CsmInputFilter* input = SimpleMenu::smGetInputFilter();
	input->Register();
	SimpleMenu::CsmLuaState* lua = new SimpleMenu::CsmLuaState();
	CIwResGroup* sampleGroup =  IwGetResManager()->LoadGroup("menu/mainmenu.group");
	SimpleMenu::CsmMenu* m = (SimpleMenu::CsmMenu*)sampleGroup->GetResNamed("mainmenu", "CsmMenu");
	m->Initialize(lua);
	input->RegisterReceiver(m);

	{

		for (;;)
		{
			s3eDeviceYield(0);
			s3eKeyboardUpdate();

			bool result = true;
			if	(
				(result == false) ||
				(s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN) ||
				(s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN) ||
				(s3eDeviceCheckQuitRequest())
				)
				break;


			//IwGxClear(IW_GX_DEPTH_BUFFER_F);
			IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
			m->Update(30);
			m->Render();

			IwGxFlush();
			IwGxSwapBuffers();
		}
	}
	input->UnRegisterReceiver(m);
	IwGetResManager()->DestroyGroup(sampleGroup);
	delete lua;
	input->UnRegister();

	SimpleMenu::smTerminate();
	fthTerminate();
	IwGraphicsTerminate();
	IwResManagerTerminate();
	IwGxTerminate();
}