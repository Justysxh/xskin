#ifndef __UIKEY_EVENT_H_INCLUDED__
#define __UIKEY_EVENT_H_INCLUDED__

#include "UIEvent.h"

class XSKIN_API UIKeyEvent : public UIEvent
{
public:
			UIKeyEvent(UIResponser* pSource, EventType nType, int nKeytCode, int nRepeatCount, int nFlags);
			~UIKeyEvent();

public:
	int		GetKeyCode() const;
	int		GetRepeatCount() const;

private:
	int m_nKeyCode;
	int m_nRepeatCount;
};

#endif //! __UIKEY_EVENT_H_INCLUDED__