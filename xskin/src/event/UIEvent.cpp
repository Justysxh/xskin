#include "stdafx.h"
#include "../../include/event/UIEvent.h"
#include "../../include/core/UIResponser.h"

UIEvent::UIEvent(UIResponser* pSource, EventType nType, int nFlags)
{
	m_pSource = pSource;
	m_nTimestamp = ::GetTickCount();
	m_nFlags = nFlags;
	m_nType = nType;
}

UIEvent::~UIEvent()
{

}

UIResponser* UIEvent::GetSource() const
{
	return m_pSource;
}

int UIEvent::GetTimestamp() const
{
	return m_nTimestamp;
}

UIEvent::EventType UIEvent::GetType() const
{
	return m_nType;
}

int UIEvent::GetFlags() const
{
	return m_nFlags;
}

int UIEvent::GetKeyStateFlags()
{
	int nFlags = 0;
	if(::GetKeyState(VK_CONTROL) < 0)
		nFlags |= UIEvent::CTRL_MASK;
	if(::GetKeyState(VK_SHIFT) < 0)
		nFlags |= UIEvent::SHIFT_MASK;
	if(::GetKeyState(VK_MENU) < 0)
		nFlags |= UIEvent::ALT_MASK;

	return nFlags;
}