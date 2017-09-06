#include "stdafx.h"
#include "../../include/view/UITextView.h"
#include "../../include/view/UIScrollBar.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/event/UIWheelEvent.h"

IMPLEMENT_XML_NODE(UITextView)

BEGIN_XML_PROPERTY(UITextView)
	SET_BOOL(multiline, m_bMultiline)
	SET_SKIN(vscroll:skin, m_pVScrollbar)
END_XML_PROPERTY()

UITextView::UITextView(UIView* parent)
 : UIControl(parent)
{
	m_nScroll = 0;
	m_bMultiline = FALSE;
	m_alignment.cx = UIAlignment::ALIGN_LEFT;
	m_alignment.cy = UIAlignment::ALIGN_VCENTER;
	m_pVScrollbar = new UIScrollBar(this);
	m_pVScrollbar->SetSizePolicy(UISizePolicy::PREFERRED, UISizePolicy::EXPANDING);
	m_pVScrollbar->SetDirection(Direction::VERTICAL);
	m_pVScrollbar->SetRange(0, 10000);
	m_pVScrollbar->SetVisible(FALSE);
}

UITextView::~UITextView()
{
	
}

UISize UITextView::GetPreferredSize() const
{
	UISize size;
	size.cx = m_pFont->GetStringWidth(m_szText);
	size.cy = m_pFont->GetHeight();

	if(m_bkgnd.IsValid())
	{
		size.cx = max(size.cx, m_bkgnd.Width());
		size.cy = max(size.cy, m_bkgnd.Height());
	}

	size.cx += m_rcPadding.left + m_rcPadding.right;
	size.cy += m_rcPadding.top + m_rcPadding.bottom;

	return size;
}

void UITextView::OnLayout()
{
	if(!m_bMultiline)
	{
		m_pVScrollbar->SetVisible(FALSE);
	}
	else
	{
		m_pVScrollbar->SetVisible(TRUE);
		UISize szPref = m_pVScrollbar->GetPreferredSize();
		UIRect rect;
		rect.right = m_rcBounds.Width();
		rect.left = rect.right - szPref.cx;
		rect.top = 0;
		rect.bottom = m_rcBounds.Height();
		m_pVScrollbar->SetBounds(rect);
	}

	__super::OnLayout();
}

void UITextView::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
	COLORREF clrOld = ::SetTextColor(hdc, m_clrText);
	rcPaint.left += m_rcPadding.left;
	rcPaint.right -= m_rcPadding.right;
	rcPaint.top += m_rcPadding.top;
	rcPaint.bottom -= m_rcPadding.bottom;

	if(m_bMultiline)
	{
		UISize szPref = m_pVScrollbar->GetSize();
		rcPaint.right -= szPref.cx;
		UIRender::MeasureString(hdc, rcPaint, m_szText, m_alignment, m_bMultiline);
		m_pVScrollbar->SetViewportSize(rcPaint.Height());
		if(rcPaint.Height() <= m_rcBounds.Height())
		{
			m_pVScrollbar->SetVisible(FALSE);
			rcPaint.right += szPref.cx;
		}
		else
		{
			m_pVScrollbar->SetVisible(TRUE);
		}
	}

	rcPaint.OffsetRect(0, -m_nScroll);

	UIRender::DrawString(hdc, rcPaint, m_szText, m_alignment, m_bMultiline);
	::SetTextColor(hdc, clrOld);
	::SelectObject(hdc, hOldFont);
}

BOOL UITextView::OnMouseWheel(UIWheelEvent* event)
{
	if(m_pVScrollbar->IsVisible())
	{
		if(event->GetDelta() < 0)
			m_pVScrollbar->WheelDown();
		else
			m_pVScrollbar->WheelUp();
	}

	return TRUE;
}

BOOL UITextView::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetCommand() == ACTION_VSCROLL)
	{
		UISize szRange = m_pVScrollbar->GetRange();
		int nPos = event->GetFlags();
		double ratio = static_cast<double>((nPos - szRange.cx)) / (szRange.cy - szRange.cx);
		m_nScroll = static_cast<int>((m_pVScrollbar->GetViewportSize() - m_rcBounds.Height()) * ratio);
		SetViewportOrg(0, -m_nScroll);
	}

	return TRUE;
}