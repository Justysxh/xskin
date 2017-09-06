#include "stdafx.h"
#include "../../include/event/UIWheelEvent.h"

UIWheelEvent::UIWheelEvent(UIResponser* pSource, EventType nType, const UIPoint& point, int zDelta, int nFlags)
 : UIMouseEvent(pSource, nType, point, nFlags)
{
	m_nDelta = zDelta;
}

UIWheelEvent::~UIWheelEvent()
{

}

int UIWheelEvent::GetDelta() const
{
	return m_nDelta;
}