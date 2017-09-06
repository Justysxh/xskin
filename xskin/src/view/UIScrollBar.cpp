#include "stdafx.h"
#include "../../include/view/UIScrollBar.h"
#include "../../include/event/UIMouseEvent.h"
#include "../../include/event/UIWheelEvent.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/event/UITimerEvent.h"

UIScrollBar::UIScrollButton::UIScrollButton(UIView* parent)
 : UIControl(parent)
{
	m_nTimerID = NULL;
	m_nTickStart = 0;
}

UIScrollBar::UIScrollButton::~UIScrollButton()
{

}

BOOL UIScrollBar::UIScrollButton::EnsureDelay()
{
	return (::GetTickCount() - m_nTickStart >= AUTO_SCROLL_DELAY_MS) ? TRUE : FALSE;
}

BOOL UIScrollBar::UIScrollButton::IsFocusable() const
{
	return TRUE;
}

UISize UIScrollBar::UIScrollButton::GetPreferredSize() const
{
	return UIControl::GetPreferredSize();
}

void UIScrollBar::UIScrollButton::OnPaint(HDC hdc)
{
	if(!m_bVisible)
		return;

	UIDrawable::FillMode fm = UIDrawable::FILL_NONE;
	if(GetParent())
	{
		UIScrollBar* pScrollBar = static_cast<UIScrollBar*>(GetParent());
		fm = (pScrollBar->GetDirection() == HORIZONTAL) ? UIDrawable::FILL_REPEAT_X : UIDrawable::FILL_REPEAT_Y;
	}

	int bkState = GetBkState();
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());

	if(bkState == BS_NORMAL)
		m_bkNormal.Draw(hdc, rcPaint, fm, m_nOpacity);
	else if(bkState == BS_HOT)
		m_bkHot.Draw(hdc, rcPaint, fm, m_nOpacity);
	else if(bkState == BS_PUSHED)
		m_bkPushed.Draw(hdc, rcPaint, fm, m_nOpacity);
	else if(bkState == BS_DISABLED)
		m_bkDisabled.Draw(hdc, rcPaint, fm, m_nOpacity);
}

BOOL UIScrollBar::UIScrollButton::OnMousePressed(UIMouseEvent* event)
{
	__super::OnMousePressed(event);
	m_nTickStart = ::GetTickCount();
	m_nTimerID = StartTimer(NULL, 50);
	return FALSE;
}

BOOL UIScrollBar::UIScrollButton::OnMouseReleased(UIMouseEvent* event)
{
	__super::OnMouseReleased(event);
	m_nTickStart = 0;
	KillTimer(m_nTimerID);
	return TRUE;
}

BOOL UIScrollBar::UIScrollButton::OnMouseDragged(UIMouseEvent* event)
{
	__super::OnMouseDragged(event);
	return FALSE;
}

BOOL UIScrollBar::UIScrollButton::OnMouseWheel(UIWheelEvent* event)
{
	__super::OnMouseWheel(event);
	return FALSE;
}

IMPLEMENT_XML_NODE(UIScrollBar)

BEGIN_XML_PROPERTY(UIScrollBar)
	SET_SKIN(lineup:skin, m_pLineUp)
	SET_SKIN(linedown:skin, m_pLineDown)
	SET_SKIN(track:skin, m_pTrack)
END_XML_PROPERTY()

UIScrollBar::UIScrollBar(UIView* parent)
 : UIView(parent)
{
	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;
	m_nViewportSize = 0;
	m_nMinTrackSize = 0;
	m_nLineSize = UIApplication::Instance()->GetAssetManager()->GetGuiFont()->GetHeight();
	m_pLineUp = new UIScrollButton(this);
	m_pLineDown = new UIScrollButton(this);
	m_pTrack = new UIScrollButton(this);
	SetDirection(VERTICAL);
}

UIScrollBar::~UIScrollBar()
{
	
}

void UIScrollBar::Scroll(int nDistance)
{
	if(!IsVisible())
		return;

	double ratio = static_cast<double>(nDistance) / (m_nViewportSize - abs(nDistance));
	int nScroll = static_cast<int>((m_nMax - m_nMin) * ratio);
	int nPos = Clamp(m_nPos - nScroll, m_nMin, m_nMax);
	SetPos(nPos);
}

void UIScrollBar::SetPos(int nPos)
{
	if(!IsVisible())
		return;

	UISize szView = GetSize();
	int nSize = (m_direction == HORIZONTAL) ? szView.cx : szView.cy;
	if(m_nViewportSize > nSize)
	{
		m_nPos = Clamp(nPos, m_nMin, m_nMax);
		UpdateTrackPos();
		UIActionEvent event(this, GetID(), (m_direction == HORIZONTAL) ? ACTION_HSCROLL : ACTION_VSCROLL, m_nPos);
		UIApplication::SendEvent(&event);
	}
}

int UIScrollBar::GetPos() const
{
	return m_nPos;
}

void UIScrollBar::LineUp()
{
	Scroll(m_nLineSize);
}

void UIScrollBar::LineDown()
{
	Scroll(-m_nLineSize);
}

void UIScrollBar::PageUp()
{
	UISize size = GetSize();
	Scroll(m_direction == HORIZONTAL ? size.cx : size.cy);
}

void UIScrollBar::PageDown()
{
	UISize size = GetSize();
	Scroll(m_direction == HORIZONTAL ? -size.cx : -size.cy);
}

void UIScrollBar::WheelUp()
{
	Scroll(m_nLineSize * 3);
}

void UIScrollBar::WheelDown()
{
	Scroll(-m_nLineSize * 3);
}

void UIScrollBar::SetViewportSize(int nSize)
{
	if(m_nViewportSize == nSize)
		return;

	m_nViewportSize = nSize;
	SetPos(m_nPos);
}

int	UIScrollBar::GetViewportSize() const
{
	return m_nViewportSize;
}

void UIScrollBar::SetLineSize(int nLineSize)
{
	m_nLineSize = nLineSize;
}

int	UIScrollBar::GetLineSize() const
{
	return m_nLineSize;
}

void UIScrollBar::SetRange(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;
}

UISize UIScrollBar::GetRange() const
{
	return UISize(m_nMin, m_nMax);
}

void UIScrollBar::UpdateTrackPos()
{
	UISize size = GetSize();
	if(m_direction == HORIZONTAL && m_nViewportSize <= size.cx)
		return;

	if(m_direction == VERTICAL && m_nViewportSize <= size.cy)
		return;

	UISize szLineUp = m_pLineUp->GetSize();
	UISize szLineDown = m_pLineDown->GetSize();
	int nTrackSize = static_cast<int>(m_direction == HORIZONTAL
		? static_cast<double>(size.cx) / m_nViewportSize * (size.cx - szLineUp.cx - szLineDown.cx)
		: static_cast<double>(size.cy) / m_nViewportSize * (size.cy - szLineUp.cy - szLineDown.cy));

	nTrackSize = max(nTrackSize, m_nMinTrackSize);
	int nLimit = m_direction == HORIZONTAL ? size.cx - szLineUp.cx - szLineDown.cx
		: size.cy - szLineUp.cy - szLineDown.cy;

	int nMove = nLimit - nTrackSize;
	double ratio = static_cast<double>(m_nPos - m_nMin) / (m_nMax - m_nMin);
	UIRect rcTrack;
	if(m_direction == HORIZONTAL)
	{
		rcTrack.left = szLineUp.cx + static_cast<int>(nMove * ratio);
		rcTrack.top = 0;
		rcTrack.right = rcTrack.left + nTrackSize;
		rcTrack.bottom = size.cy;
	}
	else
	{
		rcTrack.left = 0;
		rcTrack.top = szLineUp.cy + static_cast<int>(nMove * ratio);
		rcTrack.right = size.cx;
		rcTrack.bottom = rcTrack.top + nTrackSize;
	}

	m_pTrack->SetBounds(rcTrack);
	Invalidate();
}

void UIScrollBar::SetTrackPos(const UIRect& rcTrack)
{
	UISize size = GetSize();
	UISize szLineUp = m_pLineUp->GetSize();
	UISize szLineDown = m_pLineDown->GetSize();

	int nTrackSize = static_cast<int>(m_direction == HORIZONTAL
		? static_cast<double>(size.cx) / m_nViewportSize * (size.cx - szLineUp.cx - szLineDown.cx)
		: static_cast<double>(size.cy) / m_nViewportSize * (size.cy - szLineUp.cy - szLineDown.cy));

	nTrackSize = max(nTrackSize, m_nMinTrackSize);

	int nLimit = (m_direction == HORIZONTAL ? size.cx - szLineUp.cx - szLineDown.cx
		: size.cy - szLineUp.cy - szLineDown.cy);
	int nMove = nLimit - nTrackSize;

	UIRect bound;
	double ratio = 0.0;
	if(m_direction == HORIZONTAL)
	{
		bound.top = 0;
		bound.bottom = size.cy;

		if(rcTrack.left < szLineUp.cx)
			bound.left = szLineUp.cx;
		else if(rcTrack.left > size.cx - szLineDown.cx - nTrackSize)
			bound.left = size.cx - szLineDown.cx - nTrackSize;
		else
			bound.left = rcTrack.left;

		bound.right = bound.left + nTrackSize;
		ratio = (bound.left - szLineUp.cx) / static_cast<double>(nMove);
	}
	else
	{
		bound.left = 0;
		bound.right = size.cx;

		if(rcTrack.top < szLineUp.cy)
			bound.top = szLineUp.cy;
		else if(rcTrack.top > size.cy - szLineDown.cy - nTrackSize)
			bound.top = size.cy - szLineDown.cy - nTrackSize;
		else
			bound.top = rcTrack.top;

		bound.bottom = bound.top + nTrackSize;
		ratio = (bound.top - szLineUp.cy) / static_cast<double>(nMove);
	}

	m_pTrack->SetBounds(bound);
	int nLastPos = m_nPos;
	m_nPos = m_nMin + static_cast<int>((m_nMax - m_nMin) * ratio);
	Invalidate();

	if(nLastPos != m_nPos)
	{
		UIString szAction = m_direction == HORIZONTAL ? ACTION_HSCROLL : ACTION_VSCROLL;
		UIActionEvent event(this, GetID(), szAction, m_nPos);
		UIApplication::SendEvent(&event);
	}
}

void UIScrollBar::SetDirection(Direction orient)
{
	m_direction = orient;
	m_pLineUp->SetSizePolicy(UISizePolicy::PREFERRED, UISizePolicy::PREFERRED);
	m_pLineDown->SetSizePolicy(UISizePolicy::PREFERRED, UISizePolicy::PREFERRED);
	m_pTrack->SetSizePolicy(UISizePolicy::FIXED, UISizePolicy::FIXED);
}

Direction UIScrollBar::GetDirection() const
{
	return m_direction;
}

UISize UIScrollBar::GetPreferredSize() const
{
	UISize size;
	UISize szLineUp = m_pLineUp->GetPreferredSize();
	UISize szLineDown = m_pLineDown->GetPreferredSize();
	UISize szTrack = m_pTrack->GetPreferredSize();
	if(m_direction == HORIZONTAL)
	{
		size.cx = szLineUp.cx + szLineDown.cx + szTrack.cx;
		size.cy = max(max(szLineUp.cy, szLineDown.cy), szTrack.cy);
	}
	else
	{
		size.cx = max(max(szLineUp.cx, szLineDown.cx), szTrack.cx);
		size.cy = szLineUp.cy + szLineDown.cy + szTrack.cy;
	}

	return size;
}

int UIScrollBar::Clamp(int x, int a, int b)
{
	if(x <= a)
		x = a;
	else if(x >= b)
		x = b;

	return x;
}

void UIScrollBar::OnLayout()
{
	UISize size;
	UISize szParent = GetSize();

	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(!v->IsVisible())
			continue;

		UISizePolicy sp = v->GetSizePolicy();
		UISize szFixed = v->GetSize();
		UISize szPref = v->GetPreferredSize();

		if(sp.cx == UISizePolicy::FIXED)
			size.cx = szFixed.cx;
		else
			size.cx = szPref.cx;

		if(sp.cy == UISizePolicy::FIXED)
			size.cy = szFixed.cy;
		else
			size.cy = szPref.cy;

		v->SetBounds(0, 0, size.cx, size.cy);
	}
	
	if(m_direction == HORIZONTAL)
	{
		size = m_pLineUp->GetSize();
		m_pLineUp->SetBounds(0, 0, size.cx, size.cy);
		size = m_pLineDown->GetSize();
		m_pLineDown->SetBounds(szParent.cx - size.cx, 0, szParent.cx, size.cy);
	}
	else
	{
		size = m_pLineUp->GetSize();
		m_pLineUp->SetBounds(0, 0, size.cx, size.cy);
		size = m_pLineDown->GetSize();
		m_pLineDown->SetBounds(0, szParent.cy - size.cy, size.cx, szParent.cy);
	}

	m_nMinTrackSize = min(szParent.cx, szParent.cy);
	UpdateTrackPos();
	__super::OnLayout();
}

void UIScrollBar::OnPaint(HDC hdc)
{
	if(!m_bVisible)
		return;

	UIRect rcPaint;
	if(m_direction == HORIZONTAL)
	{
		rcPaint.SetRect(m_pLineUp->GetSize().cx, 0, m_rcBounds.Width() - m_pLineDown->GetSize().cx, m_rcBounds.Height());
		m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	}
	else
	{
		rcPaint.SetRect(0, m_pLineUp->GetSize().cy, m_rcBounds.Width(), m_rcBounds.Height() - m_pLineDown->GetSize().cy);
		m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_Y, m_nOpacity);
	}
}

BOOL UIScrollBar::OnMousePressed(UIMouseEvent* event)
{
	if(event->GetSource() == m_pLineUp)
	{
		LineUp();
	}
	else if(event->GetSource() == m_pLineDown)
	{
		LineDown();
	}
	else if(event->GetSource() == this)
	{
		UIPoint point = MapToView(this, event->GetLocation());
		UIRect rcTrack = m_pTrack->GetBounds();
		if(m_direction == HORIZONTAL)
		{
			if(point.x < rcTrack.left)
				PageUp();
			else if(point.x > rcTrack.right)
				PageDown();
		}
		else
		{
			if(point.y < rcTrack.top)
				PageUp();
			else if(point.y > rcTrack.bottom)
				PageDown();
		}
	}
	else if(event->GetSource() == m_pTrack)
	{
		m_ptLast = event->GetLocation();
	}
	
	return TRUE;
}

BOOL UIScrollBar::OnMouseReleased(UIMouseEvent* event)
{
	__super::OnMouseReleased(event);

	return TRUE;
}

BOOL UIScrollBar::OnMouseWheel(UIWheelEvent* event)
{
	int nDelta = event->GetDelta();
	if(nDelta < 0)
		WheelDown();
	else
		WheelUp();

	return TRUE;
}

BOOL UIScrollBar::OnMouseDragged(UIMouseEvent* event)
{
	if(event->GetSource() == m_pTrack)
	{
		UIPoint point = event->GetLocation();
		int nOffset = m_direction == HORIZONTAL ? point.x - m_ptLast.x : point.y - m_ptLast.y;
		UIRect rcTrack = m_pTrack->GetBounds();
		UIPoint ptTrack(rcTrack.left, rcTrack.top);

		if(m_direction == HORIZONTAL)
			rcTrack.OffsetRect(nOffset, 0);
		else
			rcTrack.OffsetRect(0, nOffset);

		SetTrackPos(rcTrack);

		UIPoint ptOffset = m_pTrack->GetPosition() - ptTrack;
		m_ptLast.Offset(ptOffset.x, ptOffset.y);
	}

	return TRUE;
}

BOOL UIScrollBar::OnTimer(UITimerEvent* event)
{
	if(event->GetSource() == m_pLineUp)
	{
		if(m_pLineUp->EnsureDelay())
			LineUp();
	}
	else if(event->GetSource() == m_pLineDown)
	{
		if(m_pLineDown->EnsureDelay())
			LineDown();
	}

	return TRUE;
}

BOOL UIScrollBar::OnActionPerformed(UIActionEvent* event)
{
	return FALSE;
}