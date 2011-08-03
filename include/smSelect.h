#pragma once

#include "smCanvas.h"

namespace SimpleMenu
{
	class CsmSelect : public CsmItem
	{
		public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmSelect);
		//Constructor
		CsmSelect();
		//Desctructor
		virtual ~CsmSelect();
	};
}