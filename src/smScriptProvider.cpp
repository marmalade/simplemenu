#include <IwManaged.h>
#include "smScriptProvider.h"
#include "smScriptingBinds.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	IsmScriptProvider* g_smDefaultScriptProvider = 0;
}

void CsmScriptableClassDeclaration::Inherit(CsmScriptableClassDeclaration* d)
{
	for (int i=0; i<d->GetMethodsCount(); ++i)
	{
		//m_methods.push_back(d->GetMethod(i));
		m_methods.push_back(d->GetMethod(i)->Clone());
	}
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
	static  TsmScriptableClassDeclaration<CIwManaged> d (0, "CIwManaged",
		ScriptTraits::Method("GetClassName", &CIwManaged::GetClassName),
			0);
	return &d;
}
