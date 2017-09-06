#include "stdafx.h"
#include <algorithm>
#include "../../include/view/UIRootView.h"
#include "../../include/view/UIWindow.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/animation/UIAnimation.h"
#include "../../include/event/UIFocusEvent.h"
#include "../../include/core/UIApplication.h"
#include "../../include/event/UIMouseEvent.h"
#include "../../include/event/UIWheelEvent.h"
#include "../../include/event/UIKeyEvent.h"
#include "../../include/event/UIFocusEvent.h"
#include "../../include/event/UITimerEvent.h"

BOOL TabIndexCompare(UIView* a, UIView* b) { return a->GetTabIndex() < b->GetTabIndex(); }

IMPLEMENT_XML_NODE(UIRootView)

UIRootView::UIRootView()
{
	m_pFocusedView = NULL;
	m_pFirstTabstopView = NULL;
	m_pMousePressedView = NULL;
	m_pMouseMoveView = NULL;
	m_pWindow = NULL;
	m_bAnimationSchedulable = FALSE;
	m_nAnimationTimer = 0;
}

UIRootView::~UIRootView()
{
	ViewTimers::iterator itor;
	while((itor = m_viewTimers.begin()) != m_viewTimers.end())
	{
		::KillTimer(GetWindow()->GetNativeWindow(), itor->first);
		m_viewTimers.erase(itor);
	}
}

UIWindow* UIRootView::GetWindow() const
{
	return m_pWindow;
}

void UIRootView::SetWindow(UIWindow* pWnd)
{
	m_pWindow = pWnd;
}

UIView* UIRootView::GetFocusedView() const
{
	return m_pFocusedView;
}

void UIRootView::SetFocusView(UIView* v)
{
	if(m_pFocusedView == v)
		return;

	if(!v && m_pFocusedView)
	{
		UIView* v = m_pFocusedView;
		m_pFocusedView = NULL;
		UIFocusEvent event(v, UIEvent::EVENT_FOCUS_LOST);
		UIApplication::SendEvent(&event);
		v->Invalidate();
		return;
	}

	if(v && v != this && v->IsFocusable())
	{
		if(m_pFocusedView && v != m_pFocusedView)
		{
			UIFocusEvent event(m_pFocusedView, UIEvent::EVENT_FOCUS_LOST);
			UIApplication::SendEvent(&event);
			m_pFocusedView->Invalidate();
		}

		m_pFocusedView = v;

		UIFocusEvent event(v, UIEvent::EVENT_FOCUS_GAIN);
		UIApplication::SendEvent(&event);
		v->Invalidate();
	}
}

void UIRootView::InitGlobalTabList()
{
	ViewList views;
	GetViewsWithTabstop(&views);
	if(views.size() > 0)
	{
		std::sort(views.begin(), views.end(), TabIndexCompare);
		int nCount = views.size();
		for(int x = 0; x < nCount - 1; ++x)
		{
			views[x]->SetNextFocusableView(views[x + 1]);
		}
		views[nCount - 1]->SetNextFocusableView(views[0]);
		m_pFirstTabstopView = views[0];
	}
}

UIView*	UIRootView::FindNextFocusableView()
{
	UIView* v = NULL;
	if(!m_pFocusedView)
	{
		v = m_pFirstTabstopView;
	}
	else
	{
		v = m_pFocusedView->GetNextFocusableView();

		// UIEditView can get focused by mouse clicking, but it may not have focus siblings,
		// so we switch to the first tab stop view always, this logic is applicable to other
		// views which focused by mouse clicking.
		v = v ? v : m_pFirstTabstopView;
	}

	UIView* p = v;
	while(v && !v->IsFocusable())
	{
		v = v->GetNextFocusableView();
		if(v == p) v = NULL;
	}

	return v;
}

UIView*	UIRootView::FindPrevFocusableView()
{
	UIView* v = NULL;
	if(!m_pFocusedView)
	{
		v = m_pFirstTabstopView;
	}
	else
	{
		v = m_pFocusedView->GetPrevFocusableView();
		v = v ? v : m_pFirstTabstopView;
	}

	UIView* p = v;
	while(v && !v->IsFocusable())
	{
		v = v->GetPrevFocusableView();
		if(v == p) v = NULL;
	}

	return v;
}

UIRootView* UIRootView::GetRootView()
{
	return this;
}

UINT UIRootView::SetTimer(UIView* pView, UINT_PTR uTimerID, int nInterval)
{
	if(uTimerID == 0)
		uTimerID = m_viewTimers.size() + 1;

	UINT uID = ::SetTimer(GetWindow()->GetNativeWindow(), uTimerID, nInterval, NULL);
	m_viewTimers.insert(std::make_pair(uID, pView));
	return uID;
}

void UIRootView::KillTimer(UINT_PTR uTimerID)
{
	ViewTimers::iterator itor = m_viewTimers.find(uTimerID);
	if(itor != m_viewTimers.end())
	{
		m_viewTimers.erase(itor);
		::KillTimer(GetWindow()->GetNativeWindow(), uTimerID);
	}
}

void UIRootView::ScheduleAnimation(UIView* pView)
{
	UIAnimation* pAnim = pView->GetAnimation();
	if(pAnim && !pAnim->IsAnimating())
	{
		m_animViews.insert(pView);
	}
	if(m_nAnimationTimer == 0 && m_bAnimationSchedulable)
	{
		m_nAnimationTimer = StartTimer(NULL, 15);
	}
}

UIResponser* UIRootView::GetNextResponser() const
{
	return GetWindow();
}

void UIRootView::OnLayout()
{
	UIView* v = GetChildView(0);
	if(v && v->IsVisible())
	{
		UISize size = GetSize();
		v->SetBounds(0, 0, size.cx, size.cy);
	}

	__super::OnLayout();

	if(!m_bAnimationSchedulable)
	{
		m_bAnimationSchedulable = TRUE;
		if(m_nAnimationTimer == 0 && m_animViews.size() > 0)
			m_nAnimationTimer = StartTimer(NULL, 15);
	}

	if(!IsInitialized())
		OnInit();
}

void UIRootView::OnPaint(HDC hdc)
{
	const UIBrush* hbr = UIApplication::Instance()->GetAssetManager()->GetBrush(RGB(246, 251, 254));
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	UIRender::FillRect(hdc, rcPaint, *hbr);
}

void UIRootView::OnInit()
{
	__super::OnInit();

	InitGlobalTabList();
}

void UIRootView::ProcessMouseDoubleClicked(const UIPoint& pt, int nFlags)
{
	UIView* v = GetViewForPoint(pt);
	if(v && v != this && v->IsEnabled())
	{
		UIMouseEvent event(v, UIMouseEvent::EVENT_MOUSE_DBLCLICKED, pt, nFlags);
		UIApplication::SendEvent(&event);
	}
}

void UIRootView::ProcessMousePressed(const UIPoint& pt, int nFlags)
{
	UIView* v = GetViewForPoint(pt);

	if(m_pFocusedView && v != m_pFocusedView)
		SetFocusView(NULL);

	if(v && v != this && v->IsEnabled())
	{
		m_pMousePressedView = v;

		UIMouseEvent event(v, UIMouseEvent::EVENT_MOUSE_PRESSED, pt, nFlags);
		UIApplication::SendEvent(&event);
	}
}

void UIRootView::ProcessMouseReleased(const UIPoint& pt, int nFlags)
{
	if(m_pMousePressedView)
	{
		UIMouseEvent event(m_pMousePressedView, UIEvent::EVENT_MOUSE_RELEASED, pt, nFlags);
		UIApplication::SendEvent(&event);

		UIView* v = GetViewForPoint(pt);
		if(v == m_pMousePressedView && (nFlags & UIEvent::LEFT_BUTTON_MASK))
		{
			UIMouseEvent event(m_pMousePressedView, UIEvent::EVENT_MOUSE_CLICKED, pt, nFlags);
			UIApplication::SendEvent(&event);
		}

		m_pMousePressedView = NULL;
	}
}

void UIRootView::ProcessMouseMoved(const UIPoint& pt, int nFlags)
{
	UIView* v = GetViewForPoint(pt);
	if(v && v != this && v->IsEnabled())
	{
		if(v != m_pMouseMoveView)
		{
			if(m_pMouseMoveView != NULL)
			{
				UIMouseEvent event(m_pMouseMoveView, UIEvent::EVENT_MOUSE_EXITED, pt, nFlags);
				UIApplication::SendEvent(&event);
			}

			m_pMouseMoveView = v;
			GetWindow()->ShowTooltip(pt, v->GetTooltipText());

			UIMouseEvent event(v, UIEvent::EVENT_MOUSE_ENTERED, pt, nFlags);
			UIApplication::SendEvent(&event);
		}

		UIMouseEvent event(m_pMouseMoveView, UIEvent::EVENT_MOUSE_MOVED, pt, nFlags);
		UIApplication::SendEvent(&event);
	}
	else if(m_pMouseMoveView != NULL)
	{
		GetWindow()->HideTooltip();

		UIMouseEvent event(m_pMouseMoveView, UIEvent::EVENT_MOUSE_EXITED, pt, nFlags);
		UIApplication::SendEvent(&event);
		m_pMouseMoveView = NULL;
	}
}

void UIRootView::ProcessMouseWheel(const UIPoint& pt, short zDelta, int nFlags)
{
	if(m_pFocusedView)
	{
		UIWheelEvent event(m_pFocusedView, UIEvent::EVENT_MOUSE_WHEEL, pt, zDelta, nFlags);
		UIApplication::SendEvent(&event);
	}
 }

void UIRootView::ProcessMouseDragged(const UIPoint& pt, int nFlags)
{
	if(m_pMousePressedView)
	{
		UIMouseEvent event(m_pMousePressedView, UIEvent::EVENT_MOUSE_DRAGGED, pt, nFlags);
		UIApplication::SendEvent(&event);
	}
}

void UIRootView::ProcessKeyPressed(int nKeyCode, int nRepeatCnt, int nFlags)
{
	if(nKeyCode == VK_TAB)
	{
		UIView* pFocusableView = NULL;
		if(nFlags & UIEvent::SHIFT_MASK)
			pFocusableView = FindPrevFocusableView();
		else
			pFocusableView = FindNextFocusableView();

		if(pFocusableView)
			SetFocusView(pFocusableView);
	}

	UIView* v = GetFocusedView();
	if(v && v != this && v->IsEnabled())
	{
		UIKeyEvent event(v, UIEvent::EVENT_KEY_PRESSED, nKeyCode, nRepeatCnt, nFlags);
		UIApplication::SendEvent(&event);
	}
}

void UIRootView::ProcessKeyReleased(int nKeyCode, int nRepeatCnt, int nFlags)
{
	UIView* v = GetFocusedView();
	if(v && v != this && v->IsEnabled())
	{
		UIKeyEvent event(v, UIEvent::EVENT_KEY_RELEASED, nKeyCode, nRepeatCnt, nFlags);
		UIApplication::SendEvent(&event);
	}
}

void UIRootView::ProcessChar(int nChar, int nRepeatCnt, int nFlags)
{

}

void UIRootView::ProcessTimer(UINT uTimerID)
{
	ViewTimers::iterator itor = m_viewTimers.find(uTimerID);
	if(itor != m_viewTimers.end())
	{
		UITimerEvent event(itor->second, uTimerID);
		UIApplication::SendEvent(&event);
	}
}

void UIRootView::ProcessCursor(HWND hWnd, UINT nHitTest, UINT message)
{
	if(m_pMouseMoveView != NULL)
	{
		HCURSOR hCursor = m_pMouseMoveView->GetCursor();
		if(hCursor)
		{
			::SetCursor(m_pMouseMoveView->GetCursor());
			return;
		}
	}

	::DefWindowProc(hWnd, WM_SETCURSOR, reinterpret_cast<WPARAM>(hWnd), MAKELPARAM(nHitTest, message));
}

BOOL UIRootView::OnTimer(UITimerEvent* event)
{
	if(event->GetSource() == this)
	{
#if _DEBUG
		static DWORD nFrames = 0;
		static DWORD nLastTime = ::GetTickCount();
		nFrames++;
		if(::GetTickCount() - nLastTime >= 1000)
		{
			DWORD nFPS = static_cast<DWORD>(nFrames * 1000.0 / (::GetTickCount() - nLastTime));
			nLastTime = GetTickCount();
			nFrames = 0;
			XTRACE(_T("fps: %d"), nFPS);
		}
#endif
		AnimationViews animFinishViews;
		AnimationViews::iterator itor = m_animViews.begin();
		for(; itor != m_animViews.end(); ++itor)
		{
			UIAnimation* pAnim = (*itor)->GetAnimation();
			if(pAnim)
			{
				if(pAnim->IsAnimating())
					pAnim->OnAnimationStepped();
				else
					animFinishViews.insert(*itor);
			}
		}

		while((itor = animFinishViews.begin()) != animFinishViews.end())
		{
			m_animViews.erase(*itor);
			animFinishViews.erase(*itor);
		}

		if(m_animViews.size() == 0)
		{
			KillTimer(m_nAnimationTimer);
			m_nAnimationTimer = 0;
		}
	}

	return TRUE;
}