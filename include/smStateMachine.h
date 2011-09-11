#pragma once

#include "smMenu.h"
#include "smInput.h"
#include "smScriptProvider.h"

namespace SimpleMenu
{
	typedef bool (*smUpdateCallback)(CsmMenu*m, void*arg);

	enum smStateMachineState
	{
		SMACTION_DO_ACTION_OR_MENU,
		SMACTION_LOAD,
		SMACTION_FADE_IN,
		SMACTION_FADE_OUT,
		SMACTION_RELEASE
	};
	struct smStateMachineStateContext
	{
		CsmInputQueue* inputQueue;
		smStateMachineState state;
		int ms;
		clock_t prev;
	};

	class CsmStateMachine
	{
	public:
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();

		static void CloseAll();
		static void Close();
		static void OpenMenuAtGroup(const char*s);
		static void Alert(const char*s,const char*m);
	};

	class CsmState
	{
	public:
		virtual ~CsmState() {};
		virtual void Load(CsmInputQueue* inputQueue) { };
		virtual bool FadeIn(int ms) { return false; };
		virtual void Update(int ms) { };
		virtual void Prepare() { };
		virtual void Render() { };
		virtual bool FadeOut(int ms) { return false;  };
		virtual void Release() { };

	};

	class CsmMenuState: public CsmState
	{
	protected:
		CsmMenu* menu;
	public:
		CsmMenuState();
		virtual ~CsmMenuState();
		virtual bool FadeIn(int ms);
		virtual void Update(int ms);
		virtual void Prepare();
		virtual void Render();
		virtual bool FadeOut(int ms);
	};

	class CsmGroupMenuState: public CsmMenuState
	{
	protected:
		CIwResGroup* group;
		std::string groupName;
		std::string menuName;
	public:
		CsmGroupMenuState();
		CsmGroupMenuState(const char* _groupName, const char* _menuName);
		virtual ~CsmGroupMenuState();
		virtual void Load(CsmInputQueue* inputQueue);
		virtual void Release();
	};

	void smStateMachinePush(CsmState*);
	void smStateMachinePop();
	CsmState* smStateMachinePeek();
	void smStateMachineLoop(CsmInputFilter* input);
	void smStateMachineIterate(smStateMachineStateContext* context);

	void smStateMachineInit();
	void smStateMachineTerminate();
}