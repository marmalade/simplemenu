#pragma once

#include <IwGeom.h>
#include <string>
#include <IwString.h>
//#include "lu.h"
#include "smLua.h"
#include "smScriptProvider.h"

namespace SimpleMenu
{
	class CsmLuaStateRoot;

	class CsmLuaState : public CIwManaged, public IsmScriptProvider
	{
	protected:
		lua_State *L;
		int currentArg;
		int numRes;
		CIwArray<uint32> openScripts;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmLuaState);
		//Constructor
		CsmLuaState();
		//Desctructor
		virtual ~CsmLuaState();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		void Initialize();

		virtual void RegisterClass(CsmScriptableClassDeclaration* c);
		virtual void Eval(const char*s, void* instance, CsmScriptableClassDeclaration*c);

		virtual void ReturnNil();
		virtual void Return();
		virtual void Return(int i);
		virtual void Return(bool i);
		virtual void Return(float i);
		virtual void Return(const char*);
		virtual void Return(void*,CsmScriptableClassDeclaration*);
		//virtual void Return(const std::string&);
		//virtual void Return(const CIwStringL&);
		//virtual void Return(const CIwStringS&);

		virtual int PopArgInt();
		virtual bool PopArgBool();
		virtual float PopArgFloat();
		virtual const char* PopArgStr();
		virtual void* PopArgClass(CsmScriptableClassDeclaration*);

	protected:
		static int smLuaMethodCall(lua_State *L);
		static int smLuaStaticMethodCall(lua_State *L);
		int CallMethod(CsmScriptableClassDeclaration* c,CsmScriptableMethodDeclaration* m,void*o);
#ifdef IW_BUILD_RESOURCES

		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}