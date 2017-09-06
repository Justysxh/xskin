#ifndef __UIACTION_EVENT_H_INCLUDED__
#define __UIACTION_EVENT_H_INCLUDED__

#include "UIEvent.h"
#include "../core/UIString.h"

#define ACTION_CLICK			_T("click")			// clicking on object, nFlag is unused.
#define ACTION_CHECK			_T("check")			// checked state changed, nFlag is TRUE if selected otherwise FALSE.
#define ACTION_SELECT			_T("select")		// selection changed, nFlag is selected index.
#define ACTION_SCROLL			_T("scroll")		// view content scrolled, high word of nFlag is x-offset, low word is y-offset
#define ACTION_HSCROLL			_T("hscroll")		// horizontal scrollbar scrolled, nFlag is scroll value.
#define ACTION_VSCROLL			_T("vscroll")		// vertical scrollbar scrolled, nFlag is scroll value.
#define ACTION_VALUE_CHANGED	_T("valuechanged")	// value changed, such as text etc, nFlag is unused.

class XSKIN_API UIActionEvent : public UIEvent
{
public:
				UIActionEvent(UIResponser* pSource, const UIString& szID, const UIString& szCommand, int nFlags = 0);
				~UIActionEvent();

public:
	UIString	GetID() const;
	UIString	GetCommand() const;

private:
	UIString m_szID;
	UIString m_szCommand;
};
#endif //! __UIACTION_EVENT_H_INCLUDED__