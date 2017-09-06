#ifndef __UITIMER_EVENT_H_INCLUDED__
#define __UITIMER_EVENT_H_INCLUDED__

#include "UIEvent.h"

class XSKIN_API UITimerEvent : public UIEvent
{
public:
			UITimerEvent(UIResponser* pSource, UINT uTimerID, int nFlags = 0);
			~UITimerEvent();
	
public:
	UINT	GetTimerID() const;

private:
	UINT m_uTimerID;
};
#endif //! __UITIMER_EVENT_H_INCLUDED__