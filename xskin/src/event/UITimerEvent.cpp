#include "stdafx.h"
#include "../../include/event/UITimerEvent.h"

UITimerEvent::UITimerEvent(UIResponser* pSource, UINT uTimerID, int nFlags /* = 0 */)
 : UIEvent(pSource, UIEvent::EVENT_TIMER, nFlags)
{
	m_uTimerID = uTimerID;
}

UITimerEvent::~UITimerEvent()
{

}

UINT UITimerEvent::GetTimerID() const
{
	return m_uTimerID;
}
