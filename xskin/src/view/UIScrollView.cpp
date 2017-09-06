#include "stdafx.h"
#include "../../include/view/UIScrollView.h"
#include "../../include/view/UIScrollBar.h"
#include "../../include/event/UIWheelEvent.h"
#include "../../include/event/UIKeyEvent.h"
#include "../../include/event/UIActionEvent.h"

IMPLEMENT_XML_NODE(UIScrollView)

BEGIN_XML_PROPERTY(UIScrollView)
	SET_BOOL(vscroll, m_bVScrollVisible)
	SET_BOOL(hscroll, m_bHScrollVisible)
	SET_SKIN(hscroll:skin, m_pHScrollBar)
	SET_SKIN(vscroll:skin, m_pVScrollBar)
END_XML_PROPERTY()


UIScrollView::UIScrollView(UIView* parent)
 : UIView(parent)
{
	m_bHScrollVisible = TRUE;
	m_bVScrollVisible = TRUE;
	m_pHScrollBar = new UIScrollBar(this);
	m_pHScrollBar->SetVisible(FALSE);
	m_pHScrollBar->SetRange(0, 10000);
	m_pHScrollBar->SetDirection(HORIZONTAL);
	m_pVScrollBar = new UIScrollBar(this);
	m_pVScrollBar->SetVisible(FALSE);
	m_pVScrollBar->SetRange(0, 10000);
	m_pVScrollBar->SetDirection(VERTICAL);
}

UIScrollView::~UIScrollView()
{

}

UISize UIScrollView::GetPageSize() const
{
	return m_szPage;
}

void UIScrollView::SetContentView(UIView* v)
{
	if(GetChildViewCount() < 3)
		AddChildView(v);
}

UIView* UIScrollView::GetContentView()
{
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(v != m_pHScrollBar && v != m_pVScrollBar)
			return v;
	}

	return NULL;
}

void UIScrollView::RemoveContentView()
{
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(v != m_pHScrollBar && v != m_pVScrollBar)
		{
			RemoveChildView(v);
			delete v;
		}
	}
	
}

void UIScrollView::SetScrollUnit(int nUint)
{
	m_pHScrollBar->SetLineSize(nUint);
	m_pVScrollBar->SetLineSize(nUint);
}

void UIScrollView::ScrollToTop()
{
	UISize size = m_pVScrollBar->GetRange();
	m_pVScrollBar->SetPos(size.cx);
}

void UIScrollView::ScrollToBottom()
{
	UISize size = m_pVScrollBar->GetRange();
	m_pVScrollBar->SetPos(size.cy);
}

void UIScrollView::ScrollToLeft()
{
	UISize size = m_pVScrollBar->GetRange();
	m_pHScrollBar->SetPos(size.cx);
}

void UIScrollView::ScrollToRight()
{
	UISize size = m_pVScrollBar->GetRange();
	m_pHScrollBar->SetPos(size.cy);
}
UIPoint UIScrollView::GetOffset() const
{
	return m_ptOffset;
}

BOOL UIScrollView::IsFocusFrameVisible() const
{
	return FALSE;
}

UISize UIScrollView::GetPreferredSize() const
{
	UIRect bound;
	UIRect rcTemp;
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);

		if(!v->IsVisible())
			continue;

		UISize size;
		UIRect rcMargin = v->GetMargin();
		UISize szFixed = v->GetSize();
		UISizePolicy sp = v->GetSizePolicy();
		UISize szPref = v->GetPreferredSize();

		if(sp.cx == UISizePolicy::PREFERRED)
			size.cx = szPref.cx;
		else if(sp.cx == UISizePolicy::EXPANDING)
			size.cx = szPref.cx;
		else
			size.cx = szFixed.cx;

		if(sp.cy == UISizePolicy::PREFERRED)
			size.cy = szPref.cy;
		else if(sp.cy == UISizePolicy::EXPANDING)
			size.cy = szPref.cy;
		else
			size.cy = szFixed.cy;

		rcTemp.SetRect(0, 0, m_rcPadding.left + rcMargin.left + size.cx + rcMargin.right + m_rcPadding.right,
			m_rcPadding.top + rcMargin.top + size.cy + rcMargin.bottom + m_rcPadding.bottom);
		bound.UnionRect(bound, rcTemp);
	}

	return UISize(bound.Width(), bound.Height());
}

void UIScrollView::Scroll(int x, int y)
{
	UISize szView = GetSize();
	UIRect rcView = GetBounds();
	UISize szHScroll = m_pHScrollBar->GetSize();
	UISize szVScroll = m_pVScrollBar->GetSize();
	UISize szAvail;

	szAvail.cx = (m_bVScrollVisible && m_pVScrollBar->IsVisible()) ? szView.cx - szVScroll.cx : szView.cx;
	szAvail.cy = (m_bHScrollVisible && m_pHScrollBar->IsVisible()) ? szView.cy - szHScroll.cy : szView.cy;

	// lay out content views.
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(!v->IsVisible() || v == m_pHScrollBar || v == m_pVScrollBar)
			continue;

		UISize size = v->GetSize();
		UIRect margin = v->GetMargin();

		int xStart = m_ptViewportOrg.x;
		int yStart = m_ptViewportOrg.y;
		v->SetClipRect(0, 0, abs(xStart) + szAvail.cx, abs(yStart) + szAvail.cy);
		v->SetBounds(xStart + margin.left, yStart + margin.top,
			xStart + margin.left + size.cx, yStart + margin.top + size.cy);
	}
}

void UIScrollView::ProcessScrollbar()
{
	UISize szView = GetSize();
	UISize szVisible = GetPreferredSize();

	if(m_bHScrollVisible && szView.cx > 0 && szVisible.cx > szView.cx)
		m_pHScrollBar->SetVisible(TRUE);
	else
		m_pHScrollBar->SetVisible(FALSE);

	if(m_bVScrollVisible && szView.cy > 0 && szVisible.cy > szView.cy)
		m_pVScrollBar->SetVisible(TRUE);
	else
		m_pVScrollBar->SetVisible(FALSE);

	UISize szHScroll = m_pHScrollBar->GetPreferredSize();
	UISize szVScroll = m_pVScrollBar->GetPreferredSize();
	UISize szAvail;
	szAvail.cx = (m_bVScrollVisible && m_pVScrollBar->IsVisible()) ? szView.cx - szVScroll.cx : szView.cx;
	szAvail.cy = (m_bHScrollVisible && m_pHScrollBar->IsVisible()) ? szView.cy - szHScroll.cy : szView.cy;
	m_pHScrollBar->SetBounds(0, szView.cy - szHScroll.cy, szAvail.cx, szView.cy);
	m_pVScrollBar->SetBounds(szView.cx - szVScroll.cx, 0, szView.cx, szAvail.cy);

	m_szPage.cx = m_pVScrollBar->IsVisible() ? szVisible.cx + szVScroll.cx : szVisible.cx;
	m_szPage.cy = m_pHScrollBar->IsVisible() ? szVisible.cy + szHScroll.cy : szVisible.cy;

	m_pHScrollBar->SetViewportSize(m_szPage.cx);
	m_pVScrollBar->SetViewportSize(m_szPage.cy);

	m_pHScrollBar->InvalidateLayout();
	m_pVScrollBar->InvalidateLayout();
}

void UIScrollView::ProcessContent()
{
	UISize szView = GetSize();
	UIRect rcView = GetBounds();
	UISize szHScroll = m_pHScrollBar->GetSize();
	UISize szVScroll = m_pVScrollBar->GetSize();
	UISize szAvail;

	szAvail.cx = (m_bVScrollVisible && m_pVScrollBar->IsVisible()) ? szView.cx - szVScroll.cx : szView.cx;
	szAvail.cy = (m_bHScrollVisible && m_pHScrollBar->IsVisible()) ? szView.cy - szHScroll.cy : szView.cy;

	// lay out content views.
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(!v->IsVisible() || v == m_pHScrollBar || v == m_pVScrollBar)
			continue;

		UISize size;
		UISizePolicy sp = v->GetSizePolicy();
		UISize szPref = v->GetPreferredSize();
		UIRect rcMargin = v->GetMargin();
		if(sp.cx == UISizePolicy::FIXED)
			size.cx = v->GetSize().cx;
		else if(sp.cx == UISizePolicy::PREFERRED)
			size.cx = szPref.cx;
		else
			size.cx = szAvail.cx -rcMargin.left - rcMargin.right;

		if(sp.cy == UISizePolicy::FIXED)
			size.cy = v->GetSize().cy;
		else if(sp.cy == UISizePolicy::PREFERRED)
			size.cy = szPref.cy;
		else
			size.cy = szAvail.cy - rcMargin.top - rcMargin.bottom;

		int xStart = m_ptViewportOrg.x;
		int yStart = m_ptViewportOrg.y;
		v->SetClipRect(0, 0, abs(xStart) + szAvail.cx, abs(yStart) + szAvail.cy);
		v->SetBounds(xStart + rcMargin.left, yStart + rcMargin.top, xStart + rcMargin.left + size.cx, yStart + rcMargin.top + size.cy);
	}
}

void UIScrollView::OnLayout()
{
	ProcessScrollbar();
	ProcessContent();

	__super::OnLayout();
}

void UIScrollView::OnInit()
{
	if(!m_bHScrollVisible)
		m_pHScrollBar->SetVisible(FALSE);

	if(!m_bVScrollVisible)
		m_pVScrollBar->SetVisible(FALSE);

	__super::OnInit();
}

void UIScrollView::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
}

BOOL UIScrollView::OnMouseWheel(UIWheelEvent* event)
{
	if(m_bVScrollVisible)
	{
		if(event->GetDelta() < 0)
			m_pVScrollBar->WheelDown();
		else
			m_pVScrollBar->WheelUp();
	}

	return TRUE;
}

BOOL UIScrollView::OnKeyPressed(UIKeyEvent* event)
{
	if(event->GetKeyCode() == VK_LEFT)
		m_pHScrollBar->LineUp();
	else if(event->GetKeyCode() == VK_RIGHT)
		m_pHScrollBar->LineDown();
	else if(event->GetKeyCode() == VK_UP)
		m_pVScrollBar->LineUp();
	else if(event->GetKeyCode() == VK_DOWN)
		m_pVScrollBar->LineDown();
	else if(event->GetKeyCode() == VK_PRIOR)
		m_pVScrollBar->PageUp();
	else if(event->GetKeyCode() == VK_NEXT)
		m_pVScrollBar->PageDown();

	return TRUE;
}

BOOL UIScrollView::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetCommand() == ACTION_HSCROLL || event->GetCommand() == ACTION_VSCROLL)
	{
		RequestFocus();

		if(event->GetCommand() == ACTION_HSCROLL)
		{
			UISize szRange = m_pHScrollBar->GetRange();
			int nPos = event->GetFlags();
			double ratio = static_cast<double>((nPos - szRange.cx)) / (szRange.cy - szRange.cx);
			m_ptOffset.x = static_cast<int>((m_szPage.cx - m_rcBounds.Width()) * ratio);
		}
		else if(event->GetCommand() == ACTION_VSCROLL)
		{
			UISize szRange = m_pVScrollBar->GetRange();
			int nPos = event->GetFlags();
			double ratio = static_cast<double>((nPos - szRange.cx)) / (szRange.cy - szRange.cx);
			m_ptOffset.y = static_cast<int>((m_szPage.cy - m_rcBounds.Height()) * ratio);
		}

		SetViewportOrg(-m_ptOffset.x, -m_ptOffset.y);
		Scroll(m_ptViewportOrg.x, m_ptViewportOrg.y);

		UIActionEvent action(this, GetID(), ACTION_SCROLL, MAKELONG(m_ptOffset.y, m_ptOffset.x));
		UIApplication::SendEvent(&action);
		return TRUE;
	}

	return FALSE;
}