#include "stdafx.h"
#include "../../include/event/UIHierarchyEvent.h"
#include "../../include/view/UIView.h"

UIHierarchyEvent::UIHierarchyEvent(UIView* pSource, EventType nType, UIView* pParent, int nFlags)
	: UIEvent(pSource, nType, nFlags)
{
	m_pParent = pParent;
}

UIHierarchyEvent::~UIHierarchyEvent()
{

}

UIView* UIHierarchyEvent::GetChanged()
{
	return static_cast<UIView*>(GetSource());
}

UIView* UIHierarchyEvent::GetChangedParent()
{
	return m_pParent;
}