#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>

#include <string>
#include "smStyleSheet.h"
#include "smScriptProvider.h"
#include "smItem.h"
#include "smInput.h"

namespace SimpleMenu
{
	class CsmMenu : public CIwResource, public IsmInputReciever
	{
		friend class CsmItem;

		std::string onLoad;
		std::string onUpdate;
	protected:
		CsmItemList childItems;
		uint32 styleSheetHash;
		CsmStyleSettings styleSettings;
		CsmStyle style;
		CsmStyleSheet* styleSheet;
		int16 contentOffset;
		int16 scrollAnimation;
		int16 contentAreaHeight;
		int16 contentAreaOffset;
		int16 scrollAnimationAcc;
		bool isVerticalScrolled;
		CIwArray<CsmItem*> collection;
		smTouchContext* activeTouch;
		CsmItem* activeItem;
		IsmScriptProvider* scriptProvider;
		TsmIntrusiveList<CsmLazyEvent> lazyEvents;
		CIwMaterial* material;
		CIwMaterial* blendMaterial;
		bool isPreparedToRender;
		// Flag to indicate if there is enough space for content. If it's true than there isn't enough space and we should scroll the header too.
		bool isHeaderScrollable;
		smItemContext renderContext;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmMenu);
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
		//Constructor
		CsmMenu();
		//Desctructor
		virtual ~CsmMenu();

		void Initialize(IsmScriptProvider* sp);
		IsmScriptProvider* GetScriptProvider() const { return scriptProvider; };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();
		//Prepare to render
		void Prepare();

		//Render image on the screen surface
		void Render();
		CIwMaterial* GetFlatMaterial();
		CIwMaterial* GetBlendedMaterial();
		//Update
		void Update(iwfixed dt);

		void AlignBlocks();

		CsmItem* GetContent() const { return (childItems.size() > 0)?static_cast<CsmItem*>(childItems[0]):(CsmItem*)0;}
		CsmItem* GetHeader() const { return (childItems.size() > 1)?static_cast<CsmItem*>(childItems[1]):(CsmItem*)0;}
		CsmItem* GetFooter() const { return (childItems.size() > 2)?static_cast<CsmItem*>(childItems[2]):(CsmItem*)0;}
		CsmItem* GetItemById(const char*) const;
		CsmItem* GetItemByHash(uint32 h) const;
		CsmItem* FindActiveItemAt(const CIwVec2 & coord);

		uint32 GetStyleSheetHash() const { return styleSheetHash;};
		void SetStyleSheetHash(uint32 v);
		
		bool VisitForward(IsmVisitor* visitor) const;
		bool VisitBackward(IsmVisitor* visitor) const;	

		virtual bool TouchEvent(smTouchContext* smTouchContext);
		virtual bool TouchReleaseEvent(smTouchContext* smTouchContext);
		virtual bool TouchMotionEvent(smTouchContext* smTouchContext);
		virtual bool KeyPressedEvent(smKeyContext* smKeyContext);
		virtual bool KeyReleasedEvent(smKeyContext* smKeyContext);
		void SetFocusTo(CsmItem*);
		bool ScrollToItem(CsmItem*);
		void Eval(CsmItem*, const char*s);
		void AddItem(CsmItem* item);
		std::string GetInnerText() const;
		void CollectInnerTextTo(std::stringstream & s) const;
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: start block.
		virtual	void	ParseOpen(CIwTextParserITX* pParser);
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
		//Parses from text file: end block.
		virtual	void	ParseClose(CIwTextParserITX* pParser);
		//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
		virtual	void	ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild);
#endif
	};

#ifdef IW_BUILD_RESOURCES
	class CsmMenuResHandler : public CIwResHandler
	{
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmMenuResHandler);

		//Constructor
		CsmMenuResHandler();
		//Desctructor
		virtual ~CsmMenuResHandler();

		//Buid resource method
		virtual CIwResource*	Build(const CIwStringL& pathname);
	};
#endif
}