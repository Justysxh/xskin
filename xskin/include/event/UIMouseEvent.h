#ifndef __UIMOUSE_EVENT_H_INCLUDED__
#define __UIMOUSE_EVENT_H_INCLUDED__

#include "UIEvent.h"
#include "../core/UIPoint.h"

class XSKIN_API UIMouseEvent : public UIEvent
{
public:
				UIMouseEvent(UIResponser* pSource, EventType nType, const UIPoint& point, int nFlags);
				~UIMouseEvent();

public:
	UIPoint		GetLocation() const;
	BOOL		IsCtrlDown() const;
	BOOL		IsShiftDown() const;
	BOOL		IsAltDown() const;

private:
	UIPoint m_pos;
};
#endif //! __UIMOUSE_EVENT_H_INCLUDED__