#include "stdafx.h"
#include "../../include/event/UIActionEvent.h"

UIActionEvent::UIActionEvent(UIResponser* pSource, const UIString& szID, const UIString& szCommand, int nFlags)
	: UIEvent(pSource, EVENT_ACTION, nFlags)
{
	m_szID = szID;
	m_szCommand = szCommand;
}

UIActionEvent::~UIActionEvent()
{

}

UIString UIActionEvent::GetID() const
{
	return m_szID;
}

UIString UIActionEvent::GetCommand() const
{
	return m_szCommand;
}