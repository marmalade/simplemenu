#pragma once

#include <string>
#include <smScriptProvider.h>

namespace SimpleMenu
{
	namespace ScriptTraits
	{
		template <class T> class StripPointer { public: typedef T STRIPPED; };
		template <class T> class StripPointer<T*> { public: typedef T STRIPPED; };

		CsmScriptableClassDeclaration* GetManagedInstanceClassDescription(CIwManaged* t);
		template <class T> inline CsmScriptableClassDeclaration* GetInstanceClassDescription(T*t) { return t->GetInstanceClassDescription(); }
		template <> inline CsmScriptableClassDeclaration* GetInstanceClassDescription<CIwManaged>(CIwManaged*t) { return GetManagedInstanceClassDescription(t); }

		template <typename T> inline T FetchArgument(IsmScriptProvider* system) { return (T)system->PopArgClass(StripPointer<T>::STRIPPED::GetClassDescription()); };
		template <> inline int FetchArgument<int>(IsmScriptProvider* system) { return system->PopArgInt(); };
		template <> inline float FetchArgument<float>(IsmScriptProvider* system) { return system->PopArgFloat(); };
		template <> inline double FetchArgument<double>(IsmScriptProvider* system) { return system->PopArgFloat(); };
		template <> inline const char* FetchArgument<const char *>(IsmScriptProvider* system) { return system->PopArgStr(); };
		template <> inline bool FetchArgument<bool>(IsmScriptProvider* system) { return system->PopArgBool(); };

		template <class T> inline void PushResult(IsmScriptProvider* system, T t) { system->Return(t); };
		template <> inline void PushResult<std::string>(IsmScriptProvider* system, std::string t) { system->Return(t.c_str()); };
		template <class PTR> inline void PushResult(IsmScriptProvider* system, PTR* t)
		{
			if (t)
				system->Return(t, GetInstanceClassDescription(t));
			else
				system->ReturnNil();
		};
		template <> inline void PushResult<const char>(IsmScriptProvider* system, const char* t)
		{
			system->Return(t);
		};
		template <typename R> class NoArgFunction: public CsmScriptableMethodDeclaration
		{
		public:
			typedef R(* METHOD)();
		protected:
			METHOD m;
		public:
			virtual bool IsStatic() const {return true;}
			NoArgFunction(const char* name, METHOD mm):CsmScriptableMethodDeclaration(name),m(mm) {}
			R MakeCall(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				return m();
			}
		};
		template <typename R, typename A> class OneArgFunction: public CsmScriptableMethodDeclaration
		{
		public:
			typedef R(* METHOD)(A);
		protected:
			METHOD m;
		public:
			virtual bool IsStatic() const {return true;}
			OneArgFunction(const char* name, METHOD mm):CsmScriptableMethodDeclaration(name),m(mm) {}
			R MakeCall(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				return m((A)FetchArgument<A>(system));
			}
		};
		template <typename R, typename A1, typename A2> class TwoArgFunction: public CsmScriptableMethodDeclaration
		{
		public:
			typedef R(* METHOD)(A1,A2);
		protected:
			METHOD m;
		public:
			virtual bool IsStatic() const {return true;}
			TwoArgFunction(const char* name, METHOD mm):CsmScriptableMethodDeclaration(name),m(mm) {}
			R MakeCall(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				A1 a1 = FetchArgument<A1>(system);
				return m(a1, FetchArgument<A2>(system));
			}
		};
		template <typename R, typename A1, typename A2, typename A3> class ThreeArgFunction: public CsmScriptableMethodDeclaration
		{
		public:
			typedef R(* METHOD)(A1,A2,A3);
		protected:
			METHOD m;
		public:
			virtual bool IsStatic() const {return true;}
			ThreeArgFunction(const char* name, METHOD mm):CsmScriptableMethodDeclaration(name),m(mm) {}
			R MakeCall(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				A1 a1 = FetchArgument<A1>(system);
				A2 a2 = FetchArgument<A1>(system);
				return m(a1, a2, FetchArgument<A3>(system));
			}
		};
		template <class T, class R, typename mmm> class NoArgsMethod: public CsmScriptableMethodDeclaration
		{
		public:
			typedef mmm METHOD;
			//typedef R (T::* METHOD)();
		protected:
			METHOD m;
		public:
			NoArgsMethod(const char* name, METHOD mm):CsmScriptableMethodDeclaration(name),m(mm) {}
			R MakeCall(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				T* i = ((T*)instance);
				return (i->*m)();
			}
		};
		template <class T, class R, typename A, typename mmm> class OneArgsMethod: public CsmScriptableMethodDeclaration
		{
		public:
			typedef mmm METHOD;
		protected:
			METHOD m;
		public:
			OneArgsMethod(const char* name, METHOD mm):CsmScriptableMethodDeclaration(name),m(mm) {}
			R MakeCall(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				T* i = ((T*)instance);
				return (i->*m)(FetchArgument<A>(system));
			}
		};
		template <class T, class R, typename A1, typename A2, typename mmm> class TwoArgsMethod: public CsmScriptableMethodDeclaration
		{
		public:
			typedef mmm METHOD;
		protected:
			METHOD m;
		public:
			TwoArgsMethod(const char* name, METHOD mm):CsmScriptableMethodDeclaration(name),m(mm) {}
			R MakeCall(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				T* i = ((T*)instance);
				A1 a1 = FetchArgument<A1>(system);
				return (i->*m)(a1, FetchArgument<A2>(system));
			}
		};
		template <class R, class Caller> class MethodBase: public Caller
		{
		public:
			MethodBase(const char* name, typename Caller::METHOD m):Caller(name,m) {}
			virtual void Call(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				PushResult(system, Caller::MakeCall(system,cls,instance));
			}
		};
		template <class Caller> class MethodBase<void,Caller>: public Caller
		{
		public:
			MethodBase(const char* name, typename Caller::METHOD m):Caller(name,m) {}
			virtual void Call(IsmScriptProvider* system, CsmScriptableClassDeclaration* cls, void* instance)
			{
				Caller::MakeCall(system,cls,instance);
				system->Return();
			}
		};
		template <typename R> inline CsmScriptableMethodDeclaration* Method(const char* name, R (*fn) ())
		{
			return new MethodBase<R,NoArgFunction<R> >(name,fn);
		};
		template <typename R,typename A> inline CsmScriptableMethodDeclaration* Method(const char* name, R (*fn) (A))
		{
			return new MethodBase<R,OneArgFunction<R,A> >(name,fn);
		};
		template <typename R,typename A1,typename A2> inline CsmScriptableMethodDeclaration* Method(const char* name, R (*fn) (A1,A2))
		{
			return new MethodBase<R,TwoArgFunction<R,A1,A2> >(name,fn);
		};
		template <typename R,typename A1,typename A2,typename A3> inline CsmScriptableMethodDeclaration* Method(const char* name, R (*fn) (A1,A2,A3))
		{
			return new MethodBase<R,ThreeArgFunction<R,A1,A2,A3> >(name,fn);
		};
		template <class T, typename R> inline CsmScriptableMethodDeclaration* Method(const char* name, R (T::*fn) ())
		{
			return new MethodBase<R,NoArgsMethod<T,R,R (T::*) ()> >(name,fn);
		};
		template <class T, typename R> inline CsmScriptableMethodDeclaration* Method(const char* name, R (T::*fn)  () const)
		{
			return new MethodBase<R,NoArgsMethod<T,R,R (T::*)  () const> >(name,fn);
		};
		template <class T, typename R,typename A> inline CsmScriptableMethodDeclaration* Method(const char* name, R (T::*fn) (A))
		{
			return new MethodBase<R,OneArgsMethod<T,R,A,R (T::*) (A)> >(name,fn);
		};
		template <class T, typename R,typename A> inline CsmScriptableMethodDeclaration* Method(const char* name, R (T::*fn)  (A) const)
		{
			return new MethodBase<R,OneArgsMethod<T,R,A,R (T::*)  (A) const> >(name,fn);
		};
		template <class T, typename R,typename A1,typename A2> inline CsmScriptableMethodDeclaration* Method(const char* name, R (T::*fn) (A1, A2))
		{
			return new MethodBase<R,TwoArgsMethod<T,R,A1,A2,R (T::*) (A1,A2)> >(name,fn);
		};
		template <class T, typename R,typename A1,typename A2> inline CsmScriptableMethodDeclaration* Method(const char* name, R (T::*fn)  (A1,A2) const)
		{
			return new MethodBase<R,TwoArgsMethod<T,R,A1,A2,R (T::*)  (A1,A2) const> >(name,fn);
		};
	}
	template <class T> class TsmScriptableMethodDeclaration: public CsmScriptableMethodDeclaration
	{
		void (T::*m_fn) ();
	public:
		TsmScriptableMethodDeclaration(const char* name,void (T::*fn) ()):CsmScriptableMethodDeclaration(name),m_fn(fn) {}
		virtual void Call(IsmScriptProvider* s,CsmScriptableClassDeclaration*c,void* o) { (((T*)o)->*m_fn)(); }
	};

	template <class T, typename R> class TsmScriptableMethodDeclarationR: public CsmScriptableMethodDeclaration
	{
		R (T::*m_fn) ();
	public:
		TsmScriptableMethodDeclarationR(const char* name,R (T::*fn) ()):CsmScriptableMethodDeclaration(name),m_fn(fn) {}
		virtual void Call(IsmScriptProvider* s,CsmScriptableClassDeclaration*c,void* o) { s->Return( (((T*)o)->*m_fn)() ); }
	};

		template <class T> class TsmScriptableClassDeclaration: public CsmScriptableClassDeclaration
	{
		const char* m_name;
	public:
		TsmScriptableClassDeclaration(const char* name, ...);
		virtual ~TsmScriptableClassDeclaration() {}
		virtual const char* GetClassName() {return m_name;}

		virtual void* CreateInstance() {return new T();}
		virtual void DestroyInstance(void* t) { delete (T*)t;}
		virtual const char* ToString(void* t) { return GetClassName(); }

		//virtual int GetMethodsCount() { return 0; }
		//virtual CsmScriptableMethodDeclaration* GetMethod(int i) { return 0; }
		//static CsmScriptableMethodDeclaration* M( void (T::*ptr)() ) { return 0;}
	};

	template <class T> TsmScriptableClassDeclaration<T>::TsmScriptableClassDeclaration(const char* name, ...)
	{
		m_name = name;
		va_list vl;
		va_start( vl, name );
		for (;;)
		{
			CsmScriptableMethodDeclaration* arg = va_arg( vl, CsmScriptableMethodDeclaration*);
			if (!arg)
				break;
			m_methods.push_back(arg);
		}
		va_end( vl );
	}
}