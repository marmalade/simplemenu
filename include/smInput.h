#pragma once

#include "s3eKeyboard.h"
#include "s3ePointer.h"

namespace SimpleMenu
{
	struct smKeyContext
	{
		// Key that was pressed or released.
		s3eKey key;
		// Whether the key was pressed (1) or released (0).
		bool pressed;
	};
	struct smTouchContext
	{
		uint32 touchID;
		CIwVec2 firstKnownPoistion;
		CIwVec2 lastKnownPoistion;
		CIwVec2 currentPoistion;
		//CtoeEntityWeakPointer entityInFocus;
		void Init(uint32 tid);
	};
}