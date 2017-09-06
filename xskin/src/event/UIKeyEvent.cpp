#include "stdafx.h"
#include "../../include/event/UIKeyEvent.h"

UIKeyEvent::UIKeyEvent(UIResponser* pSource, EventType nType, int nKeyCode, int nRepeatCount, int nFlags)
 : UIEvent(pSource, nType, nFlags)
{
	m_nKeyCode = nKeyCode;
	m_nRepeatCount = nRepeatCount;
}

UIKeyEvent::~UIKeyEvent()
{

}

int UIKeyEvent::GetKeyCode() const
{
	return m_nKeyCode;
}

int UIKeyEvent::GetRepeatCount() const
{
	return m_nRepeatCount;
}