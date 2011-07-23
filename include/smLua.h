#pragma once
#include "simplemenu.h"

extern "C" {
#define lua_c
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace SimpleMenu
{
	void smLuaInit();
	void smLuaTerminate();
}
