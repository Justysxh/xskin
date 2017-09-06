#include "stdafx.h"
#include <OleCtl.h>
#include "../../include/core/UIApplication.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/gfx/UIBrush.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/view/UIEditView.h"
#include "../../include/view/UIRootView.h"
#include "../../include/view/UIWindow.h"
#include "../../include/view/UIScrollBar.h"

IMPLEMENT_XML_NODE(UIEditView)

BEGIN_XML_PROPERTY(UIEditView)
	SET_BOOL(transparent, m_bTransparent)
	SET_BOOL(readOnly, m_bReadOnly)
	SET_BOOL(multiline, m_bMultiline)
	SET_STRING(inputMode, m_szInputMode)
	SET_STRING(placeHolder, m_szPlaceholder)
	SET_INT(maxLength, m_nMaxLength)
END_XML_PROPERTY()

UIEditView::UIEditView(UIView* parent)
 : UIControl(parent)
{
	m_hWnd = NULL;
	m_pfnSuperWndProc = NULL;
	m_nMaxLength = 0;
	m_szInputMode = _T("");
	m_szPlaceholder = _T("");
	m_bTransparent = FALSE;
	m_bMultiline = FALSE;
	m_bReadOnly = FALSE;
	m_rcPadding.SetRect(3, 3, 3, 3);
	m_hCursor = UIApplication::Instance()->GetAssetManager()->GetCursor(UIAsset::CURSOR_IBEAM);
	SetFocusable(TRUE);
	RegisterControlClass();
}

UIEditView::~UIEditView()
{

}

void UIEditView::SetTransparent(BOOL b)
{
	m_bTransparent = b;
}

BOOL UIEditView::IsTransparent() const
{
	return m_bTransparent;
}

void UIEditView::SetMaxLength(int nMax)
{
	m_nMaxLength = nMax;
}

int UIEditView::GetMaxLength() const
{
	return m_nMaxLength;
}

void UIEditView::SetMultiLine(BOOL b)
{
	m_bMultiline = b;
}

BOOL UIEditView::IsMultiLine() const
{
	return m_bMultiline;
}

void UIEditView::SetInputMode(const UIString& szInputMode)
{
	m_szInputMode = szInputMode;
}

UIString UIEditView::GetInputMode() const
{
	return m_szInputMode;
}

void UIEditView::SetPlaceholder(const UIString& szText)
{
	m_szPlaceholder = szText;
}

UIString UIEditView::GetPlaceholder() const
{
	return m_szPlaceholder;
}

void UIEditView::Activate(BOOL b)
{
	if(b)
		RequestFocus();
	else
		ClearFocus();
}

BOOL UIEditView::IsFocusFrameVisible() const
{
	return FALSE;
}

UISize UIEditView::GetPreferredSize() const
{
	UISize szTemp = __super::GetPreferredSize();
	UISize size;
	size.cx = m_pFont->GetStringWidth(m_szText) + m_rcPadding.left + m_rcPadding.right;
	size.cy = m_pFont->GetHeight() + m_rcPadding.top + m_rcPadding.bottom;
	size.cx = max(size.cx, szTemp.cx);
	size.cy = max(size.cy, szTemp.cy);
	return size;
}

BOOL UIEditView::OnMousePressed(UIMouseEvent* event)
{
	RequestFocus();
	return TRUE;
}

BOOL UIEditView::OnFocusGain(UIFocusEvent* event)
{
	CreateControl();
	return TRUE;
}

BOOL UIEditView::OnFocusLost(UIFocusEvent* event)
{
	DestroyControl();
	return TRUE;
}

void UIEditView::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_rcControl = rcPaint;
	m_rcControl.left += m_rcPadding.left;
	m_rcControl.right -= m_rcPadding.right;
	m_rcControl.top += m_rcPadding.top;
	m_rcControl.bottom -= m_rcPadding.bottom;

	int bkState = GetBkState();
	if(bkState == BS_NORMAL)
	{
		if(IsChecked())
			m_bkPushed.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
		else
			m_bkNormal.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}
	else if(bkState == BS_HOT)
	{
		m_bkHot.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}
	else if(bkState == BS_PUSHED)
	{
		m_bkPushed.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}
	else if(bkState == BS_DISABLED)
	{
		m_bkDisabled.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}

	if(!m_hWnd)
	{

		UIString szText = m_szText;
		if(m_szInputMode == INPUT_MODE_PASSWORD && !szText.IsEmpty())
		{
			int nLen = szText.GetLength();
			szText.Empty();
			while(nLen--)
				szText.Append(_T("¡ñ"));
		}
		

		szText = szText.IsEmpty() ? m_szPlaceholder : szText;

		HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
		COLORREF clrText = (m_szText.IsEmpty() || !m_bEnabled) ? m_clrTextDisabled : m_clrText;
		COLORREF clrOld = ::SetTextColor(hdc, clrText);
		UIRender::DrawString(hdc, m_rcControl, szText, m_alignment, m_bMultiline);
		::SetTextColor(hdc, clrOld);
		::SelectObject(hdc, hOldFont);
	}
}

BOOL UIEditView::RegisterControlClass()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	if(!::GetClassInfoEx(NULL, _T("Edit"), &wc))
	{
		if(!::GetClassInfoEx(UIApplication::Instance()->GetHandle(), _T("Edit"), &wc))
		{
			XASSERT("Unable to locate window class Edit");
		}
	}
	m_pfnSuperWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = ControlProc;
	wc.hInstance = UIApplication::Instance()->GetHandle();
	wc.lpszClassName = _T("EditHostWnd");
	ATOM ret = ::RegisterClassEx(&wc);
	XASSERT(ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);

	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

BOOL UIEditView::CreateControl()
{
	if(m_hWnd && ::IsWindow(m_hWnd))
		return TRUE;

	if(m_bTransparent)
	{
		HWND hWnd = GetRootView()->GetWindow()->GetNativeWindow();
		LONG nStyle = GetWindowLong(hWnd, GWL_STYLE);
		nStyle &= ~(WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowLong(hWnd, GWL_STYLE, nStyle);
	}

	DWORD dwStyle = WS_CHILD | WS_VISIBLE;

	if(m_alignment.cx == UIAlignment::ALIGN_LEFT)
		dwStyle |= ES_LEFT;
	else if(m_alignment.cx == UIAlignment::ALIGN_RIGHT)
		dwStyle |= ES_RIGHT;
	else if(m_alignment.cx == UIAlignment::ALIGN_HCENTER)
		dwStyle |= ES_CENTER;

	if(m_bMultiline)
	{
		dwStyle |= ES_MULTILINE;
		dwStyle |= ES_WANTRETURN;
		dwStyle |= ES_AUTOVSCROLL;
		dwStyle |= WS_VSCROLL;
	}
	else
	{
		dwStyle |= ES_AUTOHSCROLL;
	}

	if(m_bReadOnly)
	{
		dwStyle |= ES_READONLY;
	}
	
	if(m_szInputMode == INPUT_MODE_PASSWORD)
		dwStyle |= ES_PASSWORD;
	else if(m_szInputMode == INPUT_MODE_NUMBER)
		dwStyle |= ES_NUMBER;
	else if(m_szInputMode == INPUT_MODE_UPPERCASE)
		dwStyle |= ES_UPPERCASE;
	else if(m_szInputMode == INPUT_MODE_LOWERCASE)
		dwStyle |= ES_LOWERCASE;

	HWND hParent = GetRootView()->GetWindow()->GetNativeWindow();
	UIRect rcWnd = MapToWindow(this, m_rcControl);
	if(!m_bMultiline)
	{
		rcWnd.top += (rcWnd.Height() - m_pFont->GetHeight()) / 2;
		rcWnd.bottom = rcWnd.top + m_pFont->GetHeight();
	}

	m_hWnd = ::CreateWindow(_T("EditHostWnd"), _T(""), dwStyle, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), hParent, NULL, NULL, this);
	if(m_hWnd)
	{
		if(m_szInputMode == INPUT_MODE_PASSWORD)
			::SendMessage(m_hWnd, EM_SETPASSWORDCHAR, static_cast<WPARAM>(_T('¡ñ')), 0L);

		if(m_nMaxLength > 0)
			::SendMessage(m_hWnd, EM_LIMITTEXT, m_nMaxLength, 0L);

		::SetWindowText(m_hWnd, m_szText);
		::SendMessage(m_hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(m_pFont->GetNativeFont()), FALSE);
		::SendMessage(m_hWnd, EM_SETMODIFY, FALSE, 0L);
		::SendMessage(m_hWnd, EM_SETSEL, 0, -1);
		::SendMessage(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN, 0);
		::SendMessage(m_hWnd, EM_SETMARGINS, EC_RIGHTMARGIN, 0);
		::SetFocus(m_hWnd);
		SetChecked(TRUE);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void UIEditView::DestroyControl()
{
	if(m_bTransparent)
	{
		HWND hWnd = GetRootView()->GetWindow()->GetNativeWindow();
		LONG nStyle = GetWindowLong(hWnd, GWL_STYLE);
		nStyle |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowLong(hWnd, GWL_STYLE, nStyle);
	}

	if(m_hWnd && ::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	SetChecked(FALSE);
	SetBkState(BS_NORMAL);
}

void UIEditView::UpdateControl()
{
	if(m_bTransparent)
	{
		UIRect rcWnd = MapToWindow(this, m_rcControl);
		HWND hParent = ::GetParent(m_hWnd);
		::InvalidateRect(hParent, &rcWnd, TRUE);
	}
}

LRESULT CALLBACK UIEditView::ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UIEditView* self = NULL;
	if(uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		self = static_cast<UIEditView*>(lpcs->lpCreateParams);
		self->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(self));
	}
	else
	{
		self = reinterpret_cast<UIEditView*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if(uMsg == WM_NCDESTROY && self != NULL)
		{
			::SetWindowLongPtr(self->m_hWnd, GWLP_USERDATA, 0L);
			self->m_hWnd = NULL;
		}
	}
	if(self != NULL)
		return self->OnControlMessage(uMsg, wParam, lParam);
	else
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT UIEditView::OnControlMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0L;
	switch(uMsg)
	{
	case OCM_COMMAND:
	{
		if(HIWORD(wParam) == EN_CHANGE)
		{
			int nLen = ::GetWindowTextLength(m_hWnd) + 1;
			LPTSTR szText = static_cast<LPTSTR>(_alloca(nLen * sizeof(TCHAR)));
			if(szText)
			{
				::GetWindowText(m_hWnd, szText, nLen);
				m_szText = szText;
				UIActionEvent event(this, GetID(), ACTION_VALUE_CHANGED);
				UIApplication::SendEvent(&event);
			}
		}
		else if(HIWORD(wParam) == EN_UPDATE || HIWORD(wParam) == EN_KILLFOCUS)
		{
			UpdateControl();
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		if(wParam & MK_LBUTTON)
			UpdateControl();

		::CallWindowProc(m_pfnSuperWndProc, m_hWnd, uMsg, wParam, lParam);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		UpdateControl();
		::CallWindowProc(m_pfnSuperWndProc, m_hWnd, uMsg, wParam, lParam);
	}
	break;
	case OCM_CTLCOLOREDIT:
	{
		HBRUSH hbr = NULL;
		::SetTextColor(reinterpret_cast<HDC>(wParam), GetTextColor());
		if(m_bTransparent)
		{
			::SetBkMode(reinterpret_cast<HDC>(wParam), TRANSPARENT);
			hbr = reinterpret_cast<HBRUSH>(::GetStockObject(HOLLOW_BRUSH));
		}
		else
		{
			hbr = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
		}
		
		lRes = reinterpret_cast<LRESULT>(hbr);
	}
	break;
	case WM_KEYDOWN:
	{
		UpdateControl();

		if(wParam == VK_TAB)
		{
			::SendMessage(::GetParent(m_hWnd), uMsg, wParam, lParam);
		}
		else if(wParam == 'A')
		{
			if(::GetKeyState(VK_CONTROL) & 0x8000)
				::SendMessage(m_hWnd, EM_SETSEL, 0, -1);
		}

		::CallWindowProc(m_pfnSuperWndProc, m_hWnd, uMsg, wParam, lParam);
	}
	break;
	default:
		lRes = ::CallWindowProc(m_pfnSuperWndProc, m_hWnd, uMsg, wParam, lParam);
	}
	
	return lRes;
}