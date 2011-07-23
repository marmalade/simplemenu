#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "string"
#include "smItem.h"
#include "smMenu.h"

namespace SimpleMenu
{
	class CsmClickable : public CsmItem
	{
		class CsmLazyClick: public CsmLazyEvent
		{
			CsmClickable* m_c;
		public:
			CsmLazyClick(CsmClickable*c):m_c(c){}
			virtual void Send(){ 
				if (m_c->onClick.size() > 0)
					m_c->GetRoot()->Eval(m_c, m_c->onClick.c_str());
			}
		};

	protected:
		std::string onClick;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmClickable);
		//Constructor
		CsmClickable();
		//Desctructor
		virtual ~CsmClickable();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		virtual void Prepare(smItemContext* renderContext,int16 width);
		//Render image on the screen surface
		virtual void Render(smItemContext* renderContext);
		virtual uint32 GetElementNameHash();
		virtual bool IsActive() const {return true;}
		virtual void TouchReleased(smTouchContext* touchContext);
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}