#ifndef __UIWINDOW_H_INCLUDED__
#define __UIWINDOW_H_INCLUDED__

#include <Windows.h>
#include <CommCtrl.h>
#include "../core/UIResponser.h"
#include "../core/UIString.h"
#include "../core/AutoPtr.h"
#include "../core/UIRect.h"
#include "../core/UIPoint.h"

class UIView;
class UIRootView;
class UIBitmap;
class UIEvent;
class XSKIN_API UIWindow : public UIResponser
{
public:
	enum SizeType
	{
		MAXIMIZED = 0,
		MINIMIZED,
		RESTORED
	};

					UIWindow(UIWindow* pParent = NULL);
					~UIWindow();

public:
	BOOL			Create();
	const UIRect&	GetBounds() const;
	void			SetBounds(const UIRect& rc);
	void			SetBounds(int l, int t, int r, int b);
	void			SetPos(int x, int y);
	void			SetPos(const UIPoint& pt);
	void			CenterWindow();
	void			Show();
	void			ShowNoActive();
	int				ShowModal();
	void			Hide();
	void			Close();
	void			Maximize();
	void			Minimize();
	void			Restore();
	void			ShowFullscreen(BOOL b);
	void			SetAlwaysOnTop(BOOL b);
	void			Activate();
	void			SetWindowOwner(UIWindow* pOwner);
	void			Deactivate();
	void			SetCapture(BOOL b);
	void			SetIcon(HICON hIcon);
	BOOL			IsActive() const;
	BOOL			IsVisible() const;
	BOOL			IsMaximized() const;
	BOOL			IsMinimized() const;
	BOOL			IsFullscreen() const;
	HWND			GetNativeWindow() const;
	HDC				GetPaintDC() const;
	void			SetContentView(const UIString& szXmlFileName);
	void			SetContentView(UIView* v);
	UIRootView*		GetRootView() const;
	void			ShowTooltip(const UIPoint& pt, const UIString& szTooltip);
	void			HideTooltip();
	void			SetFrameVisible(BOOL b);
	BOOL			IsFrameVisible() const;
	void			SetResizeRect(const UIRect& rc);
	void			SetResizeRect(int l, int t, int r, int b);
	UIRect			GetResizeRect() const;

protected:
	LPCTSTR			GetWndClassName() const;
	virtual DWORD	GetWndStyle() const;
	virtual DWORD	GetWndExStyle() const;
	virtual LRESULT	OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	UIResponser*	GetNextResponser() const OVERRIDE;
	BOOL			Event(UIEvent* event) OVERRIDE;
	BOOL			RegisterWndClass();
	void			DoPaint();
	LRESULT			OnReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT			OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	LRESULT			OnSize(int nType, int cx, int cy);
	LRESULT			OnPaint(HDC hdc);
	LRESULT			OnEraseBkgnd(HDC hdc);
	LRESULT			OnNcHitTest(const UIPoint& pt);
	LRESULT			OnMouseMove(const UIPoint& pt, int nFlags);
	LRESULT			OnMouseWheel(const UIPoint& pt, short zDelta, int nFlags);
	LRESULT			OnMouseHover(const UIPoint& pt, int nFlags);
	LRESULT			OnMouseLeave(const UIPoint& pt, int nFlags);
	LRESULT			OnLButtonDown(const UIPoint& pt, int nFlags);
	LRESULT			OnLButtonDblClicked(const UIPoint& pt, int nFlags);
	LRESULT			OnNCLButtonDown(const UIPoint& pt, int nFlags);
	LRESULT			OnMButtonDown(const UIPoint& pt, int nFlags);
	LRESULT			OnRButtonDown(const UIPoint& pt, int nFlags);
	LRESULT			OnLButtonUp(const UIPoint& pt, int nFlags);
	LRESULT			OnMButtonUp(const UIPoint& pt, int nFlags);
	LRESULT			OnRButtonUp(const UIPoint& pt, int nFlags);
	LRESULT			OnKeyDown(int nKeyCode, int nRepeatCnt, int nFlags);
	LRESULT			OnKeyUp(int nKeyCode, int nRepeatCnt, int nFlags);
	LRESULT			OnChar(int nChar, int nRepeatCnt, int nFlags);
	LRESULT			OnTimer(UINT uTimerID);
	LRESULT			OnKillFocus(HWND hNewWnd);
	LRESULT			OnSetFocus(HWND hOldWnd);
	virtual void 	OnCreate();
	virtual void 	OnDestroy();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	struct SavedWindowInfo
	{
		BOOL bMaximized;
		LONG nStyle;
		LONG nExStyle;
		UIRect rcRect;
	} m_sif;

	struct SavedWndStyle
	{
		LONG nStyle;
		LONG nExStyle;
	} m_sws;
	
	UIWindow* m_pParent;
	HWND m_hWnd;
	HWND m_hTooltip;
	HDC m_hDC;
	UIRect m_rcBound;
	UIRect m_rcResize;
	BOOL m_bFullscreen;
	BOOL m_bTrackingMouse;
	BOOL m_bFrameVisible;
	const UIBitmap* m_pOffscreen;
	const UIBitmap* m_pIcon;
	AutoPtr<UIRootView> m_pRootView;
	TOOLINFO m_toolinfo;
};
#endif //! __UIWINDOW_H_INCLUDED__