#ifndef __UIEDIT_VIEW_H_INCLUDED__
#define __UIEDIT_VIEW_H_INCLUDED__

#include "UIControl.h"
#include "../core/AutoPtr.h"

#define INPUT_MODE_NUMBER		_T("number")
#define INPUT_MODE_UPPERCASE	_T("upperCase")
#define INPUT_MODE_LOWERCASE	_T("lowerCase")
#define INPUT_MODE_PASSWORD		_T("password")

class UIBrush;
class UIScrollBar;
class XSKIN_API UIEditView : public UIControl
{
	DECLARE_XML_NODE(UIEditView)
	DECLARE_XML_PROPERTY()
public:
				UIEditView(UIView* parent = NULL);
				~UIEditView();

public:
	void		SetTransparent(BOOL b);
	BOOL		IsTransparent() const;
	void		SetMaxLength(int nMax);
	int			GetMaxLength() const;
	void		SetReadOnly(BOOL b);
	BOOL		IsReadOnly() const;
	void		SetMultiLine(BOOL b);
	BOOL		IsMultiLine() const;
	void		SetInputMode(const UIString& szInputMode);
	UIString	GetInputMode() const;
	void		SetPlaceholder(const UIString& szText);
	UIString	GetPlaceholder() const;
	void		Activate(BOOL b) OVERRIDE;
	BOOL		IsFocusFrameVisible() const OVERRIDE;
	UISize		GetPreferredSize() const OVERRIDE;

protected:
	BOOL		OnMousePressed(UIMouseEvent* event) OVERRIDE;
	BOOL		OnFocusGain(UIFocusEvent* event) OVERRIDE;
	BOOL		OnFocusLost(UIFocusEvent* event) OVERRIDE;
	void		OnPaint(HDC hdc) OVERRIDE;

	BOOL		RegisterControlClass();
	BOOL		CreateControl();
	void		DestroyControl();
	void		UpdateControl();
	LRESULT		OnControlMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	BOOL m_bTransparent;
	BOOL m_bMultiline;
	BOOL m_bReadOnly;
	UIString m_szInputMode;
	UIString m_szPlaceholder;
	int m_nMaxLength;
	UIRect m_rcControl;
	WNDPROC m_pfnSuperWndProc;
};
#endif //! __UIEDIT_VIEW_H_INCLUDED__