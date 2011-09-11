#pragma once

namespace SimpleMenu
{
	template<class T,typename TAG=void> class TsmIntrusiveList;

	template<class T,typename TAG=void> class TsmIntrusiveListItem
	{
		friend class TsmIntrusiveList<T,TAG>;
	protected:
		TsmIntrusiveList<T,TAG>*	listContainer;
		TsmIntrusiveListItem<T,TAG>* prevSibling;
		TsmIntrusiveListItem<T,TAG>* nextSibling;
	public:
		inline TsmIntrusiveListItem():listContainer(0),prevSibling(0),nextSibling(0){}
		inline virtual ~TsmIntrusiveListItem(){Detach();}

		inline T* GetPrevious() const {return static_cast<T*>(prevSibling); }
		inline T* GetNext() const {return static_cast<T*>(nextSibling); }
		inline TsmIntrusiveList<T,TAG>* GetListConainer() const {return listContainer; }

		void AttachHead(TsmIntrusiveList<T,TAG>* node);
		void AttachTail(TsmIntrusiveList<T,TAG>* node);
		void SwapNext();
		void SwapPrev();
		void InsertBefore(TsmIntrusiveListItem<T,TAG>* node);
		void InsertAfter(TsmIntrusiveListItem<T,TAG>* node);
		void Detach();
	};

	template<class T,typename TAG> class TsmIntrusiveList
	{
		friend class TsmIntrusiveListItem<T,TAG>;
	protected:
		TsmIntrusiveListItem<T,TAG>* firstChild;
		TsmIntrusiveListItem<T,TAG>* lastChild;
	public:
		inline TsmIntrusiveList():firstChild(0),lastChild(0){}

		inline T* GetFirstChild() const {return static_cast<T*>(firstChild); }
		inline T* GetLastChild() const {return static_cast<T*>(lastChild); }

		inline void AttachHead(TsmIntrusiveListItem<T,TAG>* node) { if(node) node->AttachHead(this);}
		inline void AttachTail(TsmIntrusiveListItem<T,TAG>* node) { if(node) node->AttachTail(this);}
	};

	template<class T,typename TAG> inline void TsmIntrusiveListItem<T,TAG>::AttachTail(TsmIntrusiveList<T,TAG>* list)
	{
		Detach();
		listContainer = list;
		if (listContainer)
		{
			prevSibling = listContainer->lastChild;
			nextSibling = 0;
			listContainer->lastChild = this;
			if (!listContainer->firstChild)
				listContainer->firstChild = this;
			else
				prevSibling->nextSibling = this;
		}
	};


	template<class T,typename TAG> inline void TsmIntrusiveListItem<T,TAG>::AttachHead(TsmIntrusiveList<T,TAG>* list)
	{
		Detach();
		listContainer = list;
		if (listContainer)
		{
			prevSibling = 0;
			nextSibling = listContainer->firstChild;
			listContainer->firstChild = this;
			if (!listContainer->lastChild)
				listContainer->lastChild = this;
			else
				nextSibling->prevSibling = this;
		}
	};


	template<class T,typename TAG> inline void TsmIntrusiveListItem<T,TAG>::Detach()
	{
		if (!listContainer) return;
		if (listContainer->lastChild == this) listContainer->lastChild = prevSibling;
		if (listContainer->firstChild == this) listContainer->firstChild = nextSibling;
		if (nextSibling) nextSibling->prevSibling = prevSibling;
		if (prevSibling) prevSibling->nextSibling = nextSibling;
		listContainer = 0;
	};

}