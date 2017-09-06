#include "stdafx.h"
#include "../../include/event/UIMouseEvent.h"

UIMouseEvent::UIMouseEvent(UIResponser* pSource, EventType nType, const UIPoint& pt, int nFlags)
 : UIEvent(pSource, nType, nFlags)
{
	m_pos = pt;
}

UIMouseEvent::~UIMouseEvent()
{

}

UIPoint UIMouseEvent::GetLocation() const
{
	return m_pos;
}

BOOL UIMouseEvent::IsCtrlDown() const
{
	return GetFlags() & CTRL_MASK ? TRUE : FALSE;
}

BOOL UIMouseEvent::IsShiftDown() const
{
	return GetFlags() & SHIFT_MASK ? TRUE : FALSE;
}

BOOL UIMouseEvent::IsAltDown() const
{
	return GetFlags() & ALT_MASK ? TRUE : FALSE;
}