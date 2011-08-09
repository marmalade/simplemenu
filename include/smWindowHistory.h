#pragma once

#include "smMenu.h"
#include "smInput.h"
#include "smScriptProvider.h"

namespace SimpleMenu
{
	typedef bool (*smUpdateCallback)(CsmMenu*m, void*);

	void smShowMenu(CsmMenu* m, CsmInputFilter* input, IsmScriptProvider* script, smUpdateCallback callback, void* context);

	void smOpenWaitDialog(const char* s, smUpdateCallback callback, void* context);

	void smOpenMenuAtGroup(const char*);
}