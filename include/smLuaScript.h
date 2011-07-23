#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "smLua.h"

namespace SimpleMenu
{
	class CsmLuaScript : public CIwResource
	{
	private:
		char* buffer;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmLuaScript);
		//Constructor
		CsmLuaScript();
		//Desctructor
		virtual ~CsmLuaScript();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		void AssignBuffer(char* buf);
		const char* GetText() const {return buffer;}
	};

#ifdef IW_BUILD_RESOURCES
	class CsmLuaScriptResHandler : public CIwResHandler
	{
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmLuaScriptResHandler);

		//Constructor
		CsmLuaScriptResHandler();
		//Desctructor
		virtual ~CsmLuaScriptResHandler();

		//Buid resource method
		virtual CIwResource*	Build(const CIwStringL& pathname);
	};
#endif
}