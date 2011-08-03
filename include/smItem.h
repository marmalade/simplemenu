#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>

#include <smInput.h>
#include <fthFont.h>
#include <smStyle.h>
#include <smStyleSheet.h>
#include <smIntrusiveList.h>
#include <smScriptProvider.h>

namespace SimpleMenu
{
	class CsmItem;

	class IsmVisitor
	{
	public:
		virtual bool Visited(CsmItem*)=0;
	};

	struct smItemContext
	{
		CsmStyleSettings* parentStyle;
		CsmStyleSheet* styleSheet;
		CIwSVec2 viewportPos;
		CIwSVec2 viewportSize;
		CIwMat transformation;
		smItemContext():parentStyle(0),styleSheet(0),transformation(CIwMat::g_Identity){};
	};
	class CsmMenu;

	class CsmLazyEvent: public TsmIntrusiveListItem<CsmLazyEvent,CsmItem>,public TsmIntrusiveListItem<CsmLazyEvent>
	{
	public:
		virtual ~CsmLazyEvent(){}
		virtual void Send(){}
	};


	class CsmItem : public CIwManaged
	{
	protected:
		CIwManagedList childItems;
		CIwSVec2 origin;
		CIwSVec2 size;
		uint32 styleClass;
		uint32 state;
		uint32 idHash;
		//CIwSVec4 margin;
		//CIwSVec4 padding;
		CsmMenu*root;
		CsmItem*parent;
		CsmStyle style;
		CsmStyleSettings combinedStyle;
		TsmIntrusiveList<CsmLazyEvent,CsmItem> lazyEvents;
		std::string onUpdate;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmItem);
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Constructor
		CsmItem();
		//Desctructor
		virtual ~CsmItem();

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		//Animate item and all child items
		virtual void Animate(iwfixed timespan);
		//Evaluates size of item and prepares all nessesary things to render it
		virtual void Prepare(smItemContext* renderContext,int16 width);
		//Render image on the screen surface
		virtual void Render(smItemContext* renderContext);

		bool IsVisible(smItemContext* renderContext);

		//Gets position of the item
		const CIwSVec2& GetOrigin() const {return origin;}
		//Gets size of the item. It's only valid after Prepare method been executed
		const CIwSVec2& GetSize() const {return size;}
		//Sets position of the item. This method should be called during parent RearrangeChildItems method
		virtual void SetOrigin(const CIwSVec2& v) { if (origin!=v) { origin=v;RearrangeChildItems(); }}
		//Gets the element name hash to select an apropriate style
		virtual uint32 GetElementNameHash();
		//Gets the element name class to select an apropriate style
		virtual uint32 GetElementClassHash();
		//Gets the element name state to select an apropriate style
		virtual uint32 GetElementStateHash();
		//Gets the element name ID hash
		uint32 GetElementIdHash() { return idHash; }

		virtual void RearrangeChildItems();
		virtual void PrepareChildItems(smItemContext* renderContext,int16 width);

		inline int16 GetMarginLeft()const {return combinedStyle.GetMarginLeft(1);}
		inline int16 GetMarginTop()const {return combinedStyle.GetMarginTop(1);}
		inline int16 GetMarginRight()const {return combinedStyle.GetMarginRight(1);}
		inline int16 GetMarginBottom()const {return combinedStyle.GetMarginBottom(1);}

		inline int16 GetBorderLeft()const {return combinedStyle.GetBorderLeft(1);}
		inline int16 GetBorderTop()const {return combinedStyle.GetBorderTop(1);}
		inline int16 GetBorderRight()const {return combinedStyle.GetBorderRight(1);}
		inline int16 GetBorderBottom()const {return combinedStyle.GetBorderBottom(1);}

		inline int16 GetPaddingLeft()const {return combinedStyle.GetPaddingLeft(1);}
		inline int16 GetPaddingTop()const {return combinedStyle.GetPaddingTop(1);}
		inline int16 GetPaddingRight()const {return combinedStyle.GetPaddingRight(1);}
		inline int16 GetPaddingBottom()const {return combinedStyle.GetPaddingBottom(1);}

		inline int16 GetContentOffsetLeft()const {return GetMarginLeft()+GetPaddingLeft()+GetBorderLeft();}
		inline int16 GetContentOffsetRight()const {return GetMarginRight()+GetPaddingRight()+GetBorderRight();}
		inline int16 GetContentOffsetTop()const {return GetMarginTop()+GetPaddingTop()+GetBorderTop();}
		inline int16 GetContentOffsetBottom()const {return GetMarginBottom()+GetPaddingBottom()+GetBorderBottom();}

		inline CsmMenu*GetRoot()const{return root;}
		inline CsmItem*GetParent()const{return parent;}
		inline CsmItem*GetChildAt(int i)const{return static_cast<CsmItem*>(childItems[i]);}
		inline int GetChildItemsCount()const{return (int)childItems.GetSize();}

		void CombineStyle(smItemContext* renderContext);
		virtual void InheritStyle(CsmStyleSettings* parentSettings);
		virtual void ApplyStyleSheet(CsmStyleSheet* styleSheet);
		virtual void ApplyStyle(CsmStyle* style);
		//Finds an active item in children
		virtual CsmItem* FindActiveItemAt(const CIwVec2 & item);
		virtual void SetFocus(bool f);
		virtual void Touch(smTouchContext* smTouchContext);
		virtual void TouchReleased(smTouchContext* smTouchContext);
		virtual void TouchCanceled(smTouchContext* smTouchContext);
		virtual void TouchMotion(smTouchContext* smTouchContext);
		//Check if element can interact with user
		virtual bool IsActive() const {return false;}

		virtual bool VisitForward(IsmVisitor* visitor);
		virtual bool VisitBackward(IsmVisitor* visitor);

		void InitTree(CsmMenu*,CsmItem*);
		void EvalUpdate();
	protected:
		void RenderBackgroud(smItemContext* renderContext);
		void RenderShadow(smItemContext* renderContext);
		void RenderBorder(smItemContext* renderContext);
		void SendLazyEvent(CsmLazyEvent*);
	public:
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: start block.
		virtual	void	ParseOpen(CIwTextParserITX* pParser);
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
		//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
		virtual	void	ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild);
#endif
	};
}