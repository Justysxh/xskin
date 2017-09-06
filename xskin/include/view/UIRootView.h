#ifndef __UIROOT_VIEW_H_INCLUDED__
#define __UIROOT_VIEW_H_INCLUDED__

#include <list>
#include <set>
#include <map>
#include "UIView.h"

class UIWindow;
class XSKIN_API UIRootView : public UIView
{
	friend class XSKIN_API UIWindow;

	DECLARE_XML_NODE(UIRootView)
public:
					UIRootView();
					~UIRootView();

public:
	UIWindow*		GetWindow() const;
	void			SetWindow(UIWindow* pWnd);
	UIView*			GetFocusedView() const;
	void			SetFocusView(UIView* v);
	void			ShowTooltip(const UIPoint& pt, const UIString& szTooltip);
	void			HideTooltip();
	UIView*			FindNextFocusableView();
	UIView*			FindPrevFocusableView();
	UINT			SetTimer(UIView* pView, UINT uTimerID, int nInterval);
	void			KillTimer(UINT_PTR uTimerID);
	UIRootView*		GetRootView() OVERRIDE;
	void			ScheduleAnimation(UIView* pView);

	void			ProcessMouseDoubleClicked(const UIPoint& pt, int nFlags);
	void			ProcessMousePressed(const UIPoint& pt, int nFlags);
	void			ProcessMouseReleased(const UIPoint& pt, int nFlags);
	void			ProcessMouseMoved(const UIPoint& pt, int nFlags);
	void			ProcessMouseWheel(const UIPoint& pt, short zDelta, int nFlags);
	void			ProcessMouseDragged(const UIPoint& pt, int nFlags);
	void			ProcessKeyPressed(int nKeyCode, int nRepeatCnt, int nFlags);
	void			ProcessKeyReleased(int nKeyCode, int nRepeatCnt, int nFlags);
	void			ProcessChar(int nChar, int nRepeatCnt, int nFlags);
	void			ProcessTimer(UINT uTimerID);
	void			ProcessCursor(HWND hWnd, UINT nHitTest, UINT message);

protected:
	void			InitGlobalTabList();
	UIResponser*	GetNextResponser() const OVERRIDE;
	void			OnLayout() OVERRIDE;
	void			OnPaint(HDC hdc) OVERRIDE;
	void			OnInit() OVERRIDE;
	BOOL			OnTimer(UITimerEvent* event) OVERRIDE;

private:
	typedef std::map<UINT, UIView*> ViewTimers;
	typedef std::set<UIView*> AnimationViews;
	UIView* m_pFocusedView;
	UIView* m_pMousePressedView;
	UIView* m_pMouseMoveView;
	UIView* m_pFirstTabstopView;
	UIWindow* m_pWindow;
	UINT m_nAnimationTimer;
	BOOL m_bAnimationSchedulable;
	ViewTimers m_viewTimers;
	AnimationViews m_animViews;
};

#endif //! __UIROOT_VIEW_H_INCLUDED__