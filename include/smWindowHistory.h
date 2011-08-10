#pragma once

#include "smMenu.h"
#include "smInput.h"
#include "smScriptProvider.h"

namespace SimpleMenu
{
	typedef bool (*smUpdateCallback)(CsmMenu*m, void*arg);

	void smShowMenu(CsmMenu* m, CsmInputFilter* input, IsmScriptProvider* script, smUpdateCallback callback, void* context);

	void smShowMenu(CsmMenu* m, smUpdateCallback callback, void* context);

	CsmMenu* smCreateDialogWindow(const char* title, const char* text);

	void smOpenWaitDialog(const char* title, const char* text, smUpdateCallback callback, void* context);
	void smAlert(const char* title, const char* text);
	void smOpenMenuAtGroup(const char*);
}