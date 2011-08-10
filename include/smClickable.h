#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include <string>
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

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		virtual void Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize);
		//Render image on the screen surface
		virtual void Render(smItemContext* renderContext);
		//Gets the element name hash to select an apropriate style
		virtual uint32 GetElementNameHash();
		//Check if element can interact with user (receive clicks and keys)
		virtual bool IsActive() const {return true;}
		virtual void TouchReleased(smTouchContext* touchContext);

		void SetOnClick(const char*s) {onClick = s;}
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	};
}