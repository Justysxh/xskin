#ifndef __UIMOUSEWHEEL_EVENT_H_INCLUDED__
#define __UIMOUSEWHEEL_EVENT_H_INCLUDED__

#include "UIMouseEvent.h"

class XSKIN_API UIWheelEvent : public UIMouseEvent
{
public:
		UIWheelEvent(UIResponser* pSource, EventType nType, const UIPoint& point, int zDelta, int nFlags);
		~UIWheelEvent();

public:
	int	GetDelta() const;

private:
	int m_nDelta;
};
#endif //! __UIMOUSEWHEEL_EVENT_H_INCLUDED__