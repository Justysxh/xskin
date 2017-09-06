#ifndef __UIFOCUS_EVENT_H_INCLUDED__
#define __UIFOCUS_EVENT_H_INCLUDED__

#include "UIEvent.h"

class XSKIN_API UIFocusEvent : public UIEvent
{
public:
	UIFocusEvent(UIResponser* pSource, EventType nType);
	~UIFocusEvent();
};
#endif //! __UIFOCUS_EVENT_H_INCLUDED__