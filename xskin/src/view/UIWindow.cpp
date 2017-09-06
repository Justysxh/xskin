#include "stdafx.h"
#include <OleCtl.h>
#include "../../include/core/UIApplication.h"
#include "../../include/core/UIAsset.h"
#include "../../include/view/UIWindow.h"
#include "../../include/view/UIView.h"
#include "../../include/view/UIRootView.h"
#include "../../include/gfx/UIBitmap.h"

UIWindow::UIWindow(UIWindow* pParent)
{
	m_hWnd = NULL;
	m_rcBound.SetRect(0, 0, 320, 240);
	m_rcResize.SetRect(4, 4, 4, 4);
	m_bFrameVisible = TRUE;
	m_bFullscreen = FALSE;
	m_bTrackingMouse = FALSE;
	m_pOffscreen = NULL;
	m_pIcon = NULL;
	m_pParent = pParent;
	m_pRootView.Reset(new UIRootView);
	m_pRootView->SetWindow(this);
	RegisterWndClass();
}

UIWindow::~UIWindow()
{
	if(::IsWindow(m_hWnd))
		::DestroyWindow(m_hWnd);

	if(m_pOffscreen)
		m_pOffscreen->Release();

	if(m_hDC)
		::DeleteDC(m_hDC);
}

UIResponser* UIWindow::GetNextResponser() const
{
	return UIApplication::Instance();
}

BOOL UIWindow::Event(UIEvent* event) OVERRIDE
{
	return FALSE;
}

BOOL UIWindow::RegisterWndClass()
{
	WNDCLASS wc = { 0 };
	wc.style = CS_VREDRAW | CS_HREDRAW;// | CS_DBLCLKS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = UIApplication::Instance()->GetHandle();
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GetWndClassName();
	ATOM ret = ::RegisterClass(&wc);
	XASSERT(ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
	return (ret != NULL) || (::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
}

LRESULT UIWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UIWindow* self = NULL;
	if(uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		self = static_cast<UIWindow*>(lpcs->lpCreateParams);
		self->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(self));
	}
	else
	{
		self = reinterpret_cast<UIWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if(uMsg == WM_NCDESTROY && self != NULL)
		{
			::SetWindowLongPtr(self->m_hWnd, GWLP_USERDATA, 0L);
			self->m_hWnd = NULL;
			self->OnDestroy();
		}
	}
	if(self != NULL)
		return self->OnWindowMessage(uMsg, wParam, lParam);
	else 
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void UIWindow::CenterWindow()
{
	XASSERT(m_hWnd && ::IsWindow(m_hWnd));

	UIRect rcWnd;
	UIRect rcArea;
	::GetWindowRect(m_hWnd, rcWnd);
	HWND hOwner = NULL;
	if(m_pParent)
		hOwner = m_pParent->GetNativeWindow();
	else
		hOwner = ::GetWindow(m_hWnd, GW_OWNER);

	if(hOwner == NULL)
		::SystemParametersInfo(SPI_GETWORKAREA, NULL, rcArea, NULL);
	else
		::GetWindowRect(hOwner, rcArea);

	int x = (rcArea.left + rcArea.right) / 2 - rcWnd.Width() / 2;
	int y = (rcArea.top + rcArea.bottom) / 2 - rcWnd.Height() / 2;
	m_rcBound.SetRect(x, y, x + m_rcBound.Width(), y + m_rcBound.Height());
	::SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void UIWindow::SetBounds(const UIRect& rc)
{
	XASSERT(m_hWnd && ::IsWindow(m_hWnd));
	m_rcBound.SetRect(rc.left, rc.top, rc.right, rc.bottom);
	::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
}

BOOL UIWindow::Create()
{
	if(m_hWnd)
		return FALSE;

	m_hWnd = ::CreateWindowEx(GetWndExStyle(), GetWndClassName(), NULL, GetWndStyle(),
		0, 0, 0, 0, NULL, NULL, UIApplication::Instance()->GetHandle(), this);
	if(m_hWnd)
	{
		//SetClassLong(m_hWnd, GCL_STYLE, GetClassLong(m_hWnd, GCL_STYLE) | CS_DROPSHADOW);

		m_hTooltip = ::CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			0, 0, 0, 0, m_hWnd, NULL, NULL, NULL);
		::ZeroMemory(&m_toolinfo, sizeof(TOOLINFO));
		m_toolinfo.cbSize = TTTOOLINFOA_V2_SIZE;
		m_toolinfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		m_toolinfo.hwnd = m_hWnd;
		m_toolinfo.uId = reinterpret_cast<UINT_PTR>(m_hWnd);
		::SendMessage(m_hTooltip, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&m_toolinfo));
		::SendMessage(m_hTooltip, TTM_SETDELAYTIME, 0, 600);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LPCTSTR UIWindow::GetWndClassName() const
{
	return TEXT("xskin_host");
}

DWORD UIWindow::GetWndStyle() const
{
	return WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

DWORD UIWindow::GetWndExStyle() const
{
	return 0L;
}

void UIWindow::SetBounds(int l, int t, int r, int b)
{
	UIRect rc(l, t, r, b);
	SetBounds(rc);
}

void UIWindow::SetPos(int x, int y)
{
	SetBounds(x, y, x + m_rcBound.Width(), y + m_rcBound.Height());
}

void UIWindow::SetPos(const UIPoint& pt)
{
	SetPos(pt.x, pt.y);
}

const UIRect& UIWindow::GetBounds() const
{
	return m_rcBound;
}

void UIWindow::Show()
{
	XASSERT(m_hWnd && ::IsWindow(m_hWnd));
	::ShowWindow(m_hWnd, SW_SHOW);
	::UpdateWindow(m_hWnd);
}

void UIWindow::ShowNoActive()
{
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::UpdateWindow(m_hWnd);
}

int UIWindow::ShowModal()
{
	XASSERT(::IsWindow(m_hWnd));
	HWND hOwner = NULL;
	if(m_pParent)
		hOwner = m_pParent->GetNativeWindow();
	else
		hOwner= ::GetWindow(m_hWnd, GW_OWNER);
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	::SetForegroundWindow(m_hWnd);
	::SetFocus(m_hWnd);
	::EnableWindow(hOwner, FALSE);
	CenterWindow();
	MSG msg = { 0 };
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	::EnableWindow(hOwner, TRUE);
	::SetForegroundWindow(hOwner);
	::SetFocus(hOwner);

	return msg.wParam;
}

void UIWindow::Hide()
{
	::ShowWindow(m_hWnd, SW_HIDE);
}

void UIWindow::Close()
{
	::DestroyWindow(m_hWnd);
}

void UIWindow::Maximize()
{
	::ShowWindow(m_hWnd, SW_MAXIMIZE);
}

void UIWindow::Minimize()
{
	::ShowWindow(m_hWnd, SW_MINIMIZE);
}

void UIWindow::Restore()
{
	::ShowWindow(m_hWnd, SW_RESTORE);
}

void UIWindow::ShowFullscreen(BOOL b)
{
	if(m_bFullscreen == b)
		return;

	m_bFullscreen = b;

	Hide();
	if(m_bFullscreen)
	{
		m_sif.nStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		m_sif.nExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		m_sif.bMaximized = IsMaximized();
		m_sif.rcRect = m_rcBound;

		HWND hDesk = ::GetDesktopWindow();
		UIRect rcDesk;
		GetWindowRect(hDesk, &rcDesk);
		SetWindowLong(m_hWnd, GWL_STYLE, m_sif.nStyle & ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU));
		SetWindowLong(m_hWnd, GWL_EXSTYLE, m_sif.nExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
		::SetWindowPos(m_hWnd, NULL, 0, 0, rcDesk.Width(), rcDesk.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_STYLE, m_sif.nStyle);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, m_sif.nExStyle);
		::SetWindowPos(m_hWnd, NULL, m_sif.rcRect.left, m_sif.rcRect.top, m_sif.rcRect.Width(), m_sif.rcRect.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

		if(m_sif.bMaximized)
			Maximize();
	}

	Show();
}

void UIWindow::SetAlwaysOnTop(BOOL b)
{
	::SetWindowPos(m_hWnd, b ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

void UIWindow::Activate()
{
	if(IsMinimized())
		::ShowWindow(m_hWnd, SW_RESTORE);

	::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetForegroundWindow(m_hWnd);
}

void UIWindow::SetWindowOwner(UIWindow* pOwner)
{
	XASSERT(::IsWindow(m_hWnd));
	XASSERT(pOwner && ::IsWindow(pOwner->GetNativeWindow()));
	::SetWindowLong(m_hWnd, GWL_HWNDPARENT, reinterpret_cast<LONG>(pOwner->GetNativeWindow()));
}

void UIWindow::Deactivate()
{
	HWND hwnd = ::GetNextWindow(m_hWnd, GW_HWNDNEXT);
	if(hwnd)
		::SetForegroundWindow(hwnd);
}

void UIWindow::SetCapture(BOOL b)
{
	if(b)
		::SetCapture(m_hWnd);
	else
		::ReleaseCapture();
}

void UIWindow::SetIcon(HICON hIcon)
{
	::SendMessage(m_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
	::SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
}

BOOL UIWindow::IsActive() const
{
	return TRUE;
}

BOOL UIWindow::IsVisible() const
{
	return ::IsWindowVisible(m_hWnd);
}

BOOL UIWindow::IsMaximized() const
{
	return ::IsZoomed(m_hWnd);
}

BOOL UIWindow::IsMinimized() const
{
	return ::IsIconic(m_hWnd);
}

BOOL UIWindow::IsFullscreen() const
{
	return m_bFullscreen;
}

HWND UIWindow::GetNativeWindow() const
{
	return m_hWnd;
}

HDC UIWindow::GetPaintDC() const
{
	return m_hDC;
}

void UIWindow::SetContentView(const UIString& szXmlFileName)
{
	UIView* v = UIView::Inflate(szXmlFileName);
	SetContentView(v);
}

void UIWindow::SetContentView(UIView* v)
{
	m_pRootView->AddChildView(v);
}

UIRootView* UIWindow::GetRootView() const
{
	return m_pRootView.Get();
}

void UIWindow::ShowTooltip(const UIPoint& pt, const UIString& szTooltip)
{
	m_toolinfo.lpszText = szTooltip.GetData();
	SendMessage(m_hTooltip, TTM_ACTIVATE, FALSE, 0L);
	SendMessage(m_hTooltip, TTM_UPDATETIPTEXT, 0, reinterpret_cast<LPARAM>(&m_toolinfo));
	SendMessage(m_hTooltip, TTM_ACTIVATE, TRUE, 0L);
}

void UIWindow::HideTooltip()
{
	SendMessage(m_hTooltip, TTM_ACTIVATE, FALSE, 0L);
}

void UIWindow::SetFrameVisible(BOOL b)
{
	XASSERT(m_hWnd && ::IsWindow(m_hWnd));

	if(m_bFrameVisible == b)
		return;

	m_bFrameVisible = b;
	BOOL bVisible = IsWindowVisible(m_hWnd);
	Hide();
	if(!m_bFrameVisible)
	{
		SetWindowLong(m_hWnd, GWL_STYLE, GetWndStyle() & ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU));
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_STYLE, GetWndStyle());
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	if(bVisible)
	Show();
}

BOOL UIWindow::IsFrameVisible() const
{
	return m_bFrameVisible;
}

void UIWindow::SetResizeRect(int l, int t, int r, int b)
{
	m_rcResize.SetRect(l, t, r, b);
}

void UIWindow::SetResizeRect(const UIRect& rc)
{
	m_rcResize = rc;
}

UIRect UIWindow::GetResizeRect() const
{
	return m_rcResize;
}

void UIWindow::OnCreate()
{
	
}

void UIWindow::OnDestroy()
{
	UIApplication::Instance()->Exit();
}

LRESULT UIWindow::OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	switch(uMsg)
	{
	case WM_CREATE:
	{
		OnCreate();
	}
	break;
	case WM_NCDESTROY:
	{
		OnDestroy();
	}
	break;
	case WM_SIZE:
	{
		lRes = OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
	}
	break;
	case WM_ERASEBKGND:
	{
		lRes = OnEraseBkgnd(reinterpret_cast<HDC>(wParam));
	}
	break;
	case WM_NCHITTEST:
	{
		lRes = OnNcHitTest(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		if(lRes == 0)
			return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	break;
	case WM_SETCURSOR:
	{
		lRes = OnSetCursor(reinterpret_cast<HWND>(wParam), LOWORD(lParam), HIWORD(lParam));
	}
	break;
	case WM_COMMAND:
	{
		lRes = OnReflect(WM_COMMAND, wParam, lParam);
	}
	break;
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	{
		lRes = OnReflect(uMsg, wParam, lParam);
	}
	break;
	case WM_MOUSEHOVER:
	{
		m_bTrackingMouse = FALSE;
		lRes = OnMouseHover(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::LEFT_BUTTON_MASK);
	}
	break;
	case WM_MOUSELEAVE:
	{
		lRes = OnMouseLeave(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::LEFT_BUTTON_MASK);

		if(m_bTrackingMouse)
			::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, (LPARAM)-1);

		m_bTrackingMouse = FALSE;
	}
	break;
	case WM_MOUSEMOVE:
	{
		if(!m_bTrackingMouse)
		{
			TRACKMOUSEEVENT tme = { 0 };
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = m_hWnd;
			tme.dwHoverTime = 300;
			TrackMouseEvent(&tme);
			m_bTrackingMouse = TRUE;
		}

		lRes = OnMouseMove(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::LEFT_BUTTON_MASK);
	}
	break;
	case WM_MOUSEWHEEL:
	{
		short zDelta = static_cast<short>(HIWORD(wParam));
		lRes = OnMouseWheel(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), zDelta,
		UIEvent::GetKeyStateFlags() | UIEvent::RIGHT_BUTTON_MASK);
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		lRes = OnLButtonDblClicked(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::LEFT_BUTTON_MASK);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		SetCapture(TRUE);
		lRes = OnLButtonDown(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::LEFT_BUTTON_MASK);
	}
	break;
	case WM_MBUTTONDOWN:
	{
		lRes = OnMButtonDown(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::MIDDLE_BUTTON_MASK);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		lRes = OnRButtonDown(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::RIGHT_BUTTON_MASK);
	}
	break;
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONDBLCLK:
	{
		lRes = OnNCLButtonDown(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::RIGHT_BUTTON_MASK);

		::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	break;
	case WM_LBUTTONUP:
	{
		SetCapture(FALSE);
		lRes = OnRButtonUp(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::LEFT_BUTTON_MASK);
	}
	break;
	case WM_MBUTTONUP:
	{
		lRes = OnRButtonUp(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::MIDDLE_BUTTON_MASK);
	}
	break;
	case WM_RBUTTONUP:
	{
		lRes = OnRButtonUp(UIPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)),
			UIEvent::GetKeyStateFlags() | UIEvent::RIGHT_BUTTON_MASK);
	}
	break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		lRes = OnKeyDown(wParam, LOWORD(lParam), UIEvent::GetKeyStateFlags());
	}
	break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		lRes = OnKeyUp(wParam, LOWORD(lParam), UIEvent::GetKeyStateFlags());
	}
	break;
	case WM_SYSCHAR:
	case WM_CHAR:
	{
		lRes = OnChar(wParam, LOWORD(lParam), UIEvent::GetKeyStateFlags());
	}
	break;
	case WM_TIMER:
	{
		lRes = OnTimer(wParam);
	}
	break;
	case WM_KILLFOCUS:
	{
		lRes = OnKillFocus(reinterpret_cast<HWND>(wParam));
	}
	break;
	case WM_SETFOCUS:
	{
		lRes = OnSetFocus(reinterpret_cast<HWND>(wParam));
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		lRes = OnPaint(ps.hdc);
		EndPaint(m_hWnd, &ps);
	}
	break;
	default:
	{
		lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	}

	return lRes;
}

LRESULT UIWindow::OnPaint(HDC hdc)
{
	UIRect rect;
	::GetClipBox(hdc, rect);
	rect.InflateRect(2, 2);
	::FillRect(m_hDC, rect, static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)));
	m_pRootView->DoPaint(m_hDC, rect);
	::BitBlt(hdc, rect.left, rect.top, rect.Width(), rect.Height(), m_hDC, rect.left, rect.top, SRCCOPY);
	return 0L;
}

LRESULT UIWindow::OnReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(lParam != NULL)
	{
		HWND hWndControl = reinterpret_cast<HWND>(lParam);
		return ::SendMessage(hWndControl, OCM__BASE + uMsg, wParam, lParam);
	}

	return 0L;
}

LRESULT UIWindow::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
	m_pRootView->ProcessCursor(hWnd, nHitTest, message);

	return TRUE;
}

LRESULT UIWindow::OnSize(int nType, int cx, int cy)
{
	UIRect rcTemp;
	::GetWindowRect(m_hWnd, rcTemp);
	m_rcBound = rcTemp;
	if(m_pOffscreen)
	{
		m_pOffscreen->Release();
		m_pOffscreen = NULL;
		::DeleteDC(m_hDC);
		m_hDC = NULL;
	}
	m_pOffscreen = UIBitmap::CreateNew(cx, cy, 32, NULL);
	m_hDC = ::CreateCompatibleDC(NULL);
	::SelectObject(m_hDC, *m_pOffscreen);
	::GetClientRect(m_hWnd, rcTemp);
	m_pRootView->SetBounds(rcTemp);
	m_pRootView->InvalidateLayout();
	return 0L;
}

LRESULT UIWindow::OnEraseBkgnd(HDC hdc)
{
	return 1L;
}

LRESULT UIWindow::OnNcHitTest(const UIPoint& pt)
{
	if(m_bFrameVisible)
		return 0L;

	UIPoint point = pt;
	::ScreenToClient(m_hWnd, &point);

	UIRect rcClient;
	::GetClientRect(m_hWnd, &rcClient);

	if(!::IsZoomed(m_hWnd))
	{
		if(point.y < rcClient.top + m_rcResize.top)
		{
			if(point.x < rcClient.left + m_rcResize.left)
				return HTTOPLEFT;

			if(point.x > rcClient.right - m_rcResize.right)
				return HTTOPRIGHT;

			return HTTOP;
		}
		else if(point.y > rcClient.bottom - m_rcResize.bottom)
		{
			if(point.x < rcClient.left + m_rcResize.left)
				return HTBOTTOMLEFT;

			if(point.x > rcClient.right - m_rcResize.right)
				return HTBOTTOMRIGHT;

			return HTBOTTOM;
		}

		if(point.x < rcClient.left + m_rcResize.left)
			return HTLEFT;

		if(point.x > rcClient.right - m_rcResize.right)
			return HTRIGHT;
	}

	return HTCLIENT;
}

LRESULT UIWindow::OnMouseMove(const UIPoint& pt, int nFlags)
{
	if(::GetCapture() == m_hWnd && (nFlags & UIEvent::LEFT_BUTTON_MASK))
		m_pRootView->ProcessMouseDragged(pt, nFlags);
	else
		m_pRootView->ProcessMouseMoved(pt, nFlags);

	return 0L;
}

LRESULT UIWindow::OnMouseWheel(const UIPoint& pt, short zDelta, int nFlags)
{
	m_pRootView->ProcessMouseWheel(pt, zDelta, nFlags);
	return 0L;
}

LRESULT UIWindow::OnMouseHover(const UIPoint& pt, int nFlags)
{
	return 0L;
}

LRESULT UIWindow::OnMouseLeave(const UIPoint& pt, int nFlags)
{
	return 0L;
}

LRESULT UIWindow::OnLButtonDown(const UIPoint& pt, int nFlags)
{
	m_pRootView->ProcessMousePressed(pt, nFlags);
	return 0L;
}

LRESULT UIWindow::OnLButtonDblClicked(const UIPoint& pt, int nFlags)
{
	m_pRootView->ProcessMouseDoubleClicked(pt, nFlags);
	return 0L;
}

LRESULT UIWindow::OnNCLButtonDown(const UIPoint& pt, int nFlags)
{
	m_pRootView->SetFocusView(NULL);
	return 0L;
}

LRESULT	UIWindow::OnMButtonDown(const UIPoint& pt, int nFlags)
{
	m_pRootView->ProcessMousePressed(pt, nFlags);
	return 0L;
}

LRESULT	UIWindow::OnRButtonDown(const UIPoint& pt, int nFlags)
{
	m_pRootView->ProcessMousePressed(pt, nFlags);
	return 0L;
}

LRESULT	UIWindow::OnLButtonUp(const UIPoint& pt, int nFlags)
{
	m_pRootView->ProcessMouseReleased(pt, nFlags);
	return 0L;
}

LRESULT	UIWindow::OnMButtonUp(const UIPoint& pt, int nFlags)
{
	m_pRootView->ProcessMouseReleased(pt, nFlags);
	return 0L;
}

LRESULT	UIWindow::OnRButtonUp(const UIPoint& pt, int nFlags)
{
	m_pRootView->ProcessMouseReleased(pt, nFlags);
	return 0L;
}

LRESULT UIWindow::OnKeyDown(int nKeyCode, int nRepeatCnt, int nFlags)
{
	m_pRootView->ProcessKeyPressed(nKeyCode, nRepeatCnt, nFlags);
	return 0L;
}

LRESULT UIWindow::OnKeyUp(int nKeyCode, int nRepeatCnt, int nFlags)
{
	m_pRootView->ProcessKeyReleased(nKeyCode, nRepeatCnt, nFlags);
	return 0L;
}

LRESULT UIWindow::OnChar(int nChar, int nRepeatCnt, int nFlags)
{
	m_pRootView->ProcessChar(nChar, nRepeatCnt, nFlags);
	return 0L;
}

LRESULT UIWindow::OnTimer(UINT uTimerID)
{
	m_pRootView->ProcessTimer(uTimerID);
	return 0L;
}

LRESULT UIWindow::OnKillFocus(HWND hNewWnd)
{
	if(::GetParent(hNewWnd) != GetNativeWindow())
		m_pRootView->SetFocusView(NULL);

	return 0L;
}

LRESULT UIWindow::OnSetFocus(HWND hOldWnd)
{
	return 0L;
}