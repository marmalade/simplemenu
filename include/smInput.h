#pragma once

#include "s3eKeyboard.h"
#include "s3ePointer.h"
#include "IwArray.h"
#include "IwGeom.h"

#include "smIntrusiveList.h"

namespace SimpleMenu
{
	struct smTouchContext;
	struct smKeyContext;

	class IsmInputReciever: public TsmIntrusiveListItem<IsmInputReciever>
	{
	public:
		virtual ~IsmInputReciever() {};

		virtual bool TouchEvent(smTouchContext* smTouchContext) = 0;
		virtual bool TouchReleaseEvent(smTouchContext* smTouchContext) = 0;
		virtual bool TouchMotionEvent(smTouchContext* smTouchContext) = 0;
		virtual bool KeyEvent(smKeyContext* smKeyContext) = 0;
	};
	class CsmInputRecieversList: public TsmIntrusiveList<IsmInputReciever>
	{
	};
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

		IsmInputReciever* receiver;
		void* receiverContextPointer;

		void Init(uint32 tid);
	};
	class CsmInputFilter
	{
		static int g_regCounter;
		static CsmInputFilter* g_regFilter;

		CIwArray<smTouchContext> activePointers;
		CIwArray<smKeyContext> activeKeys;
		CsmInputRecieversList receivers;
	public:
		CsmInputFilter();
		~CsmInputFilter();

		void RegisterReceiver(IsmInputReciever*);
		void UnRegisterReceiver(IsmInputReciever*);

		bool TouchEvent(smTouchContext*,int32,int32);
		bool TouchReleaseEvent(smTouchContext*,int32,int32);
		bool TouchMotionEvent(smTouchContext*,int32,int32);


		int32 FindTouchContextIndex(uint32 touchId) const;
		int32 GetTouchContextIndex(uint32 touchId);
		void ReleaseTouchContextAt(int32 touchIndex);

		int32 OnKeyboardKeyEvent(s3eKeyboardEvent*e);
		int32 OnPointerTouchEvent(s3ePointerTouchEvent*e);
		int32 OnPointerTouchMotionEvent(s3ePointerTouchMotionEvent*e);
		int32 OnPointerButtonEvent(s3ePointerEvent*e);
		int32 OnPointerMotionEvent(s3ePointerMotionEvent*e);

		void Register();
		void UnRegister();

		static int32 KeyboardKeyEventHandler(s3eKeyboardEvent*e, CsmInputFilter*);
		static int32 PointerTouchEventHandler(s3ePointerTouchEvent*e, CsmInputFilter*);
		static int32 PointerTouchMotionEventHandler(s3ePointerTouchMotionEvent*e, CsmInputFilter*);
		static int32 PointerButtonEventHandler(s3ePointerEvent*e, CsmInputFilter*);
		static int32 PointerMotionEventHandler(s3ePointerMotionEvent*e, CsmInputFilter*);
	};

	CsmInputFilter* smGetInputFilter();
}