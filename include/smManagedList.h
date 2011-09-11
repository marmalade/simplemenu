#pragma once

#include <IwManagedList.h>

namespace SimpleMenu
{
	template <class T> class TsmManagedList
	{
		CIwArray<T*> items;
	public:
		virtual ~TsmManagedList() {};
		void Serialise();
		void Delete();

		bool empty() const { return items.empty(); }
		T* back() const { return items.back(); }
		T* front() const { return items.front(); }
		int push_back(T*i) { return items.push_back(i);}
		T** begin() const {return items.begin();}
		T** end() const {return items.end();}
		void pop_back() { delete items.back(); return items.pop_back(); }
		size_t size() const {return items.size(); }
		T* operator [] (int const i) const { return items[i]; }
	};

	template <class T> void TsmManagedList<T>::Serialise()
	{
		if (IwSerialiseIsReading())
		{
			uint32 n;
			IwSerialiseUInt32(n);
			for (uint32 i=0; i<n; ++i)
			{
				CIwManaged*m;
				IwSerialiseManagedObject(m);
				items.push_back(static_cast<T*>(m));
			}
		}
		else
		{
			uint32 n = (uint32)size();
			IwSerialiseUInt32(n);
			for (uint32 i=0; i<n; ++i)
			{
				CIwManaged*m = static_cast<CIwManaged*>(items[i]);
				IwSerialiseManagedObject(m);
			}
		}
	}
	template <class T> void TsmManagedList<T>::Delete()
	{
		for (uint32 i=0; i<size(); ++i)
		{
			if (items[i]) 
				delete items[i];
		}
		items.clear();
	}

}