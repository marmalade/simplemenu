#include <IwDebug.h>
#include "smInput.h"

using namespace SimpleMenu;

int CsmInputFilter::g_regCounter = 0;
CsmInputFilter* CsmInputFilter::g_regFilter = 0;

void smTouchContext::Init(uint32 tid)
{
	touchID = tid;
	receiver = 0;
	receiverContextPointer = 0;
}
void smKeyContext::Init(s3eKey k)
{
	key = k;
	receiver = 0;
	receiverContextPointer = 0;
}
CsmInputFilter::CsmInputFilter()
{
}
CsmInputFilter::~CsmInputFilter()
{
	if (g_regFilter == this && g_regCounter)
		IwAssertMsg(SIMPLEMENU, false, ("Please unregister CsmInputFilter!"));
}
void CsmInputFilter::RegisterReceiver(IsmInputReciever*r)
{
	r->AttachTail(&receivers);
}
void CsmInputFilter::UnRegisterReceiver(IsmInputReciever*r)
{
	r->Detach();
}

bool CsmInputFilter::TouchEvent(smTouchContext* touchContext,int32 x,int32 y)
{
	touchContext->firstKnownPoistion.x = touchContext->lastKnownPoistion.x = touchContext->currentPoistion.x = x;
	touchContext->firstKnownPoistion.y = touchContext->lastKnownPoistion.y = touchContext->currentPoistion.y = y;

	/*context.screenSpacePoint.x = x;
	context.screenSpacePoint.y = y;
	context.filter = this;
	context.touch = &touchContext;
	context.callback =TouchEventCallback;*/
	for (IsmInputReciever* r = receivers.GetLastChild(); r; r = r->GetPrevious())
	{
		if (r->TouchEvent(touchContext))
		{
			touchContext->receiver = r;
			return true;
		}
	}

	return false;
}
bool CsmInputFilter::TouchReleaseEvent(smTouchContext* touchContext,int32 x,int32 y)
{
	touchContext->currentPoistion.x = x;
	touchContext->currentPoistion.y = y;

	if (touchContext->receiver)
	{
		touchContext->receiver->TouchReleaseEvent(touchContext);
		return true;
	}
	return false;
}
bool CsmInputFilter::TouchMotionEvent(smTouchContext* touchContext,int32 x,int32 y)
{
	bool wasHandled = false;
	touchContext->currentPoistion.x = x;
	touchContext->currentPoistion.y = y;
	if (touchContext->receiver)
	{
		touchContext->receiver->TouchMotionEvent(touchContext);
		wasHandled = true;
	}
	touchContext->lastKnownPoistion.x = touchContext->currentPoistion.x = x;
	touchContext->lastKnownPoistion.y = touchContext->currentPoistion.y = y;
	return wasHandled;
}

bool CsmInputFilter::KeyboardKeyPressedEvent(smKeyContext* item)
{
	for (IsmInputReciever* r = receivers.GetLastChild(); r; r = r->GetPrevious())
	{
		if (r->KeyPressedEvent(item))
		{
			item->receiver = r;
			return true;
		}
	}

	return false;
}
bool CsmInputFilter::KeyboardKeyReleasedEvent(smKeyContext* item)
{
	if (item->receiver)
	{
		item->receiver->KeyReleasedEvent(item);
		return true;
	}
	for (IsmInputReciever* r = receivers.GetLastChild(); r; r = r->GetPrevious())
	{
		if (r->KeyReleasedEvent(item))
		{
			item->receiver = r;
			return true;
		}
	}
	return false;
}

int32 CsmInputFilter::FindTouchContextIndex(uint32 touchId) const
{
	for (uint32 i=0; i<activePointers.size(); ++i)
	{
		smTouchContext& item = activePointers[i];
		if (item.touchID == touchId)
			return (int32)i;
	}
	return -1;
}
int32 CsmInputFilter::GetTouchContextIndex(uint32 touchId)
{
	int32 i = FindTouchContextIndex(touchId);
	if (i >= 0) return i;
	i = (int32)activePointers.size();
	activePointers.push_back();
	smTouchContext& item = activePointers[i];
	item.Init(touchId);
	return i;
}
void CsmInputFilter::ReleaseTouchContextAt(int32 touchIndex)
{
	activePointers.erase_fast(touchIndex);
}
int32 CsmInputFilter::FindKeyContextIndex(s3eKey key) const
{
	for (uint32 i=0; i<activeKeys.size(); ++i)
	{
		smKeyContext& item = activeKeys[i];
		if (item.key == key)
			return (int32)i;
	}
	return -1;
}
int32 CsmInputFilter::GetKeyContextIndex(s3eKey key)
{
	int32 i = FindKeyContextIndex(key);
	if (i >= 0) return i;
	i = (int32)activeKeys.size();
	activeKeys.push_back();
	smKeyContext& item = activeKeys[i];
	item.Init(key);
	return i;
}
void CsmInputFilter::ReleaseKeyContextAt(int32 keyIndex)
{
	activeKeys.erase_fast(keyIndex);
}


int32 CsmInputFilter::OnKeyboardKeyEvent(s3eKeyboardEvent*e)
{
	int32 index = GetKeyContextIndex(e->m_Key);
	smKeyContext* item = &activeKeys[index];
	bool wasHandled = false;
	if (e->m_Pressed)
	{
		wasHandled = KeyboardKeyPressedEvent(item);
	}
	else
	{
		wasHandled = KeyboardKeyReleasedEvent(item);
		ReleaseKeyContextAt(index);
	}
	return wasHandled?1:0;
}
int32 CsmInputFilter::OnPointerTouchEvent(s3ePointerTouchEvent*e)
{
	int32 index = GetTouchContextIndex(e->m_TouchID);
	smTouchContext* item = &activePointers[index];
	bool wasHandled;
	if (e->m_Pressed)
	{
		wasHandled = TouchEvent(item,e->m_x,e->m_y);
	}
	else
	{
		wasHandled = TouchReleaseEvent(item,e->m_x,e->m_y);
		ReleaseTouchContextAt(index);
	}
	return wasHandled?1:0;
}
int32 CsmInputFilter::OnPointerTouchMotionEvent(s3ePointerTouchMotionEvent*e)
{
	bool wasHandled;
	int32 index = FindTouchContextIndex(e->m_TouchID);
	if (index < 0) return 0;
	smTouchContext* item = &activePointers[index];
	
	wasHandled = TouchMotionEvent(item,e->m_x,e->m_y);
	return wasHandled?1:0;
}
int32 CsmInputFilter::OnPointerButtonEvent(s3ePointerEvent*e)
{
	bool wasHandled = false;
	if (e->m_Button != S3E_POINTER_BUTTON_SELECT)
		return wasHandled;

	int32 index = GetTouchContextIndex(0);
	smTouchContext* item = &activePointers[index];
	if (e->m_Pressed)
	{
		wasHandled = TouchEvent(item,e->m_x,e->m_y);
	}
	else
	{
		wasHandled = TouchReleaseEvent(item,e->m_x,e->m_y);
		ReleaseTouchContextAt(index);
	}
	return wasHandled?1:0;
}
int32 CsmInputFilter::OnPointerMotionEvent(s3ePointerMotionEvent*e)
{
	bool wasHandled = false;
	int32 index = FindTouchContextIndex(0);
	if (index < 0) return 0;
	smTouchContext* item = &activePointers[index];
	wasHandled = TouchMotionEvent(item,e->m_x,e->m_y);
	return wasHandled?1:0;
}

int32 CsmInputFilter::KeyboardKeyEventHandler(s3eKeyboardEvent*e, CsmInputFilter*p)
{
	return p->OnKeyboardKeyEvent(e);
}
int32 CsmInputFilter::PointerTouchEventHandler(s3ePointerTouchEvent*e, CsmInputFilter*p)
{
	return p->OnPointerTouchEvent(e);
}
int32 CsmInputFilter::PointerTouchMotionEventHandler(s3ePointerTouchMotionEvent*e, CsmInputFilter*p)
{
	return p->OnPointerTouchMotionEvent(e);
}
int32 CsmInputFilter::PointerButtonEventHandler(s3ePointerEvent*e, CsmInputFilter*p)
{
	return p->OnPointerButtonEvent(e);
}
int32 CsmInputFilter::PointerMotionEventHandler(s3ePointerMotionEvent*e, CsmInputFilter*p)
{
	return p->OnPointerMotionEvent(e);
}

void CsmInputFilter::Register()
{
	if (g_regCounter)
	{
		if (g_regFilter != this)
			IwAssertMsg(SIMPLEMENU,false,("Another instance of CsmInputFilter already registered"));
	}
	++g_regCounter;
	g_regFilter = this;

	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyboardKeyEventHandler, this);
	if (s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE))
	{
		s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)PointerTouchEventHandler, this);
		s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)PointerTouchMotionEventHandler, this);
	}
	else
	{
		s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)PointerButtonEventHandler, this);
		s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)PointerMotionEventHandler, this);
	}
}
void CsmInputFilter::UnRegister()
{
	if (!g_regCounter)
	{
		IwAssertMsg(SIMPLEMENU,false,("CsmInputFilter already unregistered"));
	}
	if (g_regFilter != this)
	{
		IwAssertMsg(SIMPLEMENU,false,("Other copy of CsmInputFilter was registered"));
	}
	--g_regCounter;
	if (!g_regCounter)
		g_regFilter = 0;

	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyboardKeyEventHandler);
	if (s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE))
	{
		s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)PointerTouchEventHandler);
		s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)PointerTouchMotionEventHandler);
	}
	else
	{
		s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)PointerButtonEventHandler);
		s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)PointerMotionEventHandler);
	}
}