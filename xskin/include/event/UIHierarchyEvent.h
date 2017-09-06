#ifndef __UIHIERARCHY_EVENT_H_INCLUDED__
#define __UIHIERARCHY_EVENT_H_INCLUDED__

#include "UIEvent.h"

class XSKIN_API UIView;
class XSKIN_API UIHierarchyEvent : public UIEvent
{
public:
			UIHierarchyEvent(UIView* pSource, EventType nType, UIView* pParent, int nFlags = 0);
			~UIHierarchyEvent();

public:
	UIView*	GetChanged();
	UIView*	GetChangedParent();

private:
	UIView* m_pParent;
};
#endif //! __UIHIERARCHY_EVENT_H_INCLUDED__