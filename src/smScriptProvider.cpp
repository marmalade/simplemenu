#include <IwManaged.h>
#include "smScriptProvider.h"
#include "smScriptingBinds.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	IsmScriptProvider* g_smDefaultScriptProvider = 0;
}

IsmScriptProvider* SimpleMenu::smGetDefaultScriptProvider()
{
	return g_smDefaultScriptProvider;
}

void SimpleMenu::smSetDefaultScriptProvider(IsmScriptProvider* sp)
{
	g_smDefaultScriptProvider = sp;
}


IsmScriptProvider::~IsmScriptProvider()
{
	
}


CsmScriptableClassDeclaration* SimpleMenu::ScriptTraits::GetManagedInstanceClassDescription(CIwManaged* t)
{
	static  TsmScriptableClassDeclaration<CIwManaged> d ("CIwManaged",
		ScriptTraits::Method("GetClassName", &CIwManaged::GetClassName),
			0);
	return &d;
}
