#pragma once

#include <IwArray.h>

namespace SimpleMenu
{
	class IsmScriptProvider;
	class CsmScriptableClassDeclaration;

	class CsmScriptableMethodDeclaration
	{
		const char* m_name;
	public:
		CsmScriptableMethodDeclaration(const char* name):m_name(name) {}
		virtual ~CsmScriptableMethodDeclaration() {};
		inline const char* GetMethodName() {return m_name;};
		virtual void Call(IsmScriptProvider*,CsmScriptableClassDeclaration*,void*)=0;
		virtual bool IsStatic() const {return false;}
	};

	class CsmScriptableClassDeclaration
	{
	protected:
		CIwArray<CsmScriptableMethodDeclaration*> m_methods;
	public:
		virtual void Close() { for (CIwArray<CsmScriptableMethodDeclaration*>::iterator i=m_methods.begin(); i!=m_methods.end(); ++i) delete *i; m_methods.clear_optimised(); };
		virtual ~CsmScriptableClassDeclaration() {Close();}
		virtual const char* GetClassName() =0;
		virtual void* CreateInstance() =0;
		virtual void DestroyInstance(void*) =0;
		virtual const char* ToString(void* t)=0;
		virtual int GetMethodsCount() { return m_methods.size(); }
		virtual CsmScriptableMethodDeclaration* GetMethod(int i) { return m_methods[i]; }
	};

	class IsmScriptProvider
	{
	public:
		virtual ~IsmScriptProvider();

		virtual void Eval(const char*s, void* instance, CsmScriptableClassDeclaration*c)=0;

		virtual void RegisterClass(CsmScriptableClassDeclaration*)=0;

		virtual void Return()=0;
		virtual void ReturnNil()=0;
		virtual void Return(int i)=0;
		virtual void Return(float i)=0;
		virtual void Return(const char*)=0;
		virtual void Return(bool)=0;
		virtual void Return(void*,CsmScriptableClassDeclaration*)=0;

		virtual int PopArgInt()=0;
		virtual float PopArgFloat()=0;
		virtual const char* PopArgStr()=0;
		virtual bool PopArgBool()=0;
		virtual void* PopArgClass(CsmScriptableClassDeclaration*)=0;
	};
}
#include "smScriptingBinds.h"
