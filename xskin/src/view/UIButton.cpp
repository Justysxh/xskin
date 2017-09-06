#include "stdafx.h"
#include "../../include/view/UIButton.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/gfx/UIFont.h"
#include "../../include/gfx/UIRender.h"

IMPLEMENT_XML_NODE(UIButton)

BEGIN_XML_PROPERTY(UIButton)
END_XML_PROPERTY()

UIButton::UIButton(UIView* parent)
 : UIControl(parent)
{
	m_alignment.cx = UIAlignment::ALIGN_HCENTER;
	m_alignment.cy = UIAlignment::ALIGN_VCENTER;
}

UIButton::~UIButton()
{
	
}

void UIButton::Activate(BOOL b)
{
	if(b)
	{
		UIActionEvent event(this, GetID(), ACTION_CLICK, 0);
		UIApplication::SendEvent(&event);
	}

	__super::Activate(b);
}

BOOL UIButton::OnMouseClicked(UIMouseEvent* event)
{
	UIActionEvent action(this, GetID(), ACTION_CLICK, 0);
	UIApplication::SendEvent(&action);

	return __super::OnMouseClicked(event);
}

void UIButton::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	int bkState = GetBkState();
	if(bkState == BS_NORMAL)
	{
		if(m_bChecked)
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
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
		rcPaint.OffsetRect(1, 1);
	}
	else if(bkState == BS_DISABLED)
	{
		m_bkDisabled.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}

	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
	COLORREF clrOld = ::SetTextColor(hdc, m_bEnabled ? m_clrText : m_clrTextDisabled);
	rcPaint.left += m_rcPadding.left;
	rcPaint.right -= m_rcPadding.right;
	rcPaint.top += m_rcPadding.top;
	rcPaint.bottom -= m_rcPadding.bottom;
	UIRender::DrawString(hdc, rcPaint, m_szText, m_alignment);
	::SetTextColor(hdc, clrOld);
	::SelectObject(hdc, hOldFont);
}