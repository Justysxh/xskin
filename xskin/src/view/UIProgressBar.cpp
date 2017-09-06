#include "stdafx.h"
#include "../../include/view/UIProgressBar.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/event/UIActionEvent.h"

IMPLEMENT_XML_NODE(UIProgressBar)
	
BEGIN_XML_PROPERTY(UIProgressBar)
	SET_DIRECTION(direction, m_direction)
	SET_DRAWABLE(bkFragmentImage, m_bkFragment)
	SET_INT(value, m_nValue)
	SET_INT(min, m_szRange.cx)
	SET_INT(max, m_szRange.cy)
	SET_BOOL(textVisible, m_bTextVisible)
END_XML_PROPERTY()

UIProgressBar::UIProgressBar(UIView* parent)
 : UIControl(parent)
{
	m_direction = HORIZONTAL;
	m_bFocusable = FALSE;
	m_bTextVisible = FALSE;
	m_nValue = 0;
	m_szRange.cx = 0;
	m_szRange.cy = 100;
	m_alignment.cx = UIAlignment::ALIGN_HCENTER;
	m_alignment.cy = UIAlignment::ALIGN_VCENTER;
}

UIProgressBar::~UIProgressBar()
{

}

void UIProgressBar::SetDirection(Direction dir)
{
	m_direction = dir;
}

Direction UIProgressBar::GetDirection() const
{
	return m_direction;
}

void UIProgressBar::SetTextVisible(BOOL b)
{
	if(m_bTextVisible == b)
		return;

	m_bTextVisible = b;
	Invalidate();
}

void UIProgressBar::SetValue(int nValue)
{
	if(nValue == m_nValue)
		return;

	Invalidate();
	m_nValue = Clamp(nValue, m_szRange.cx, m_szRange.cy);
	Invalidate();

	UIActionEvent event(this, GetID(), ACTION_VALUE_CHANGED, m_nValue);
	UIApplication::SendEvent(&event);
}

int UIProgressBar::GetValue() const
{
	return m_nValue;
}

void UIProgressBar::SetRange(int nMin, int nMax)
{
	m_szRange.cx = nMin;
	m_szRange.cy = nMax;
}

UISize UIProgressBar::GetRange() const
{
	return m_szRange;
}

UISize UIProgressBar::GetPreferredSize() const
{
	UIRect bound;
	UIRect temp;
	if(m_bkgnd.IsValid())
	{
		temp.SetRect(0, 0, m_bkgnd.Width(), m_bkgnd.Height());
		bound.UnionRect(bound, temp);
	}
	if(m_bkFragment.IsValid())
	{
		temp.SetRect(0, 0, m_bkFragment.Width(), m_bkFragment.Height());
		bound.UnionRect(bound, temp);
	}

	return UISize(bound.Width(), bound.Height());
}

int UIProgressBar::Clamp(int x, int a, int b)
{
	if(x < a)
		x = a;
	else if(x > b)
		x = b;

	return x;
}

void UIProgressBar::OnPaint(HDC hdc)
{
	UIDrawable::FillMode fm = (m_direction == HORIZONTAL) ? UIDrawable::FILL_REPEAT_X : UIDrawable::FILL_REPEAT_Y;
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, fm, m_nOpacity);

	UIRect rcFragment;
	if(m_direction == HORIZONTAL)
	{
		rcFragment.left = 0;
		rcFragment.top = 0;
		rcFragment.right = static_cast<int>(static_cast<double>(m_nValue - m_szRange.cx) / (m_szRange.cy - m_szRange.cx) * m_rcBounds.Width());
		rcFragment.bottom = rcPaint.bottom;
	}
	else
	{
		rcFragment.left = 0;
		rcFragment.right = rcPaint.right;
		rcFragment.top = rcPaint.bottom
			- static_cast<int>(static_cast<double>(m_nValue - m_szRange.cx) / (m_szRange.cy - m_szRange.cx) * m_rcBounds.Height());;
		rcFragment.bottom = rcPaint.bottom;
	}

	m_bkFragment.Draw(hdc, rcFragment, fm, m_nOpacity);

	if(m_bTextVisible)
	{
		m_szText.Format(_T("%d%%"), m_nValue);
		HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
		COLORREF clrOld = ::SetTextColor(hdc, m_clrText);
		UIRender::DrawString(hdc, rcPaint, m_szText, m_alignment);
		::SetTextColor(hdc, clrOld);
		::SelectObject(hdc, hOldFont);
	}
}