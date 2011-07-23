#include <IwManaged.h>
#include "smScriptProvider.h"
#include "smScriptingBinds.h"

using namespace SimpleMenu;

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
