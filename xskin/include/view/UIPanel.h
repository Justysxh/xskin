#ifndef __UIPANEL_H_INCLUDED__
#define __UIPANEL_H_INCLUDED__

#include "UIWindow.h"

class UIResponser;
class UIWheelEvent;
class UIKeyEvent;
class XSKIN_API UIPanel : public UIWindow
{
public:
					UIPanel(UIResponser* owner = NULL, UIWindow* parent = NULL);
					~UIPanel();

public:
	// Window without focus can not receive key event and mouse wheel event,
	// so we must forward it from owner view.
	void			ForwardWheelEvent(UIWheelEvent* event);
	void			ForwardKeyPressedEvent(UIKeyEvent* event);
	void			ForwardKeyReleasedEvent(UIKeyEvent* event);

protected:
	DWORD			GetWndStyle() const OVERRIDE;
	DWORD			GetWndExStyle() const OVERRIDE;
	UIResponser*	GetNextResponser() const OVERRIDE;
	LRESULT			OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;

protected:
	UIResponser* m_pOwner;
};
#endif //! __UIPANEL_H_INCLUDED__