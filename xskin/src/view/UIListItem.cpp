#include "stdafx.h"
#include "../../include/view/UIListItem.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/event/UIMouseEvent.h"
#include "../../include/event/UIActionEvent.h"

IMPLEMENT_XML_NODE(UIListItem)

BEGIN_XML_PROPERTY(UIListItem)
END_XML_PROPERTY()

UIListItem::UIListItem(UIView* parent)
 : UIButton(parent)
{
	m_alignment.cx = UIAlignment::ALIGN_LEFT;
	m_alignment.cy = UIAlignment::ALIGN_VCENTER;
	m_szText = _T("TextItem");
}

UIListItem::~UIListItem()
{

}

void UIListItem::OnPaint(HDC hdc)
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
		if(m_bChecked)
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
		else
			m_bkHot.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}
	else if(bkState == BS_PUSHED)
	{
		if(m_bChecked)
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
		else
			m_bkPushed.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
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

BOOL UIListItem::OnMousePressed(UIMouseEvent* event)
{
	if(event->GetFlags() & UIEvent::LEFT_BUTTON_MASK)
	{
		UIActionEvent action(this, GetID(), ACTION_CLICK, 0);
		UIApplication::SendEvent(&action);
	}

	return TRUE;
}

BOOL UIListItem::OnMouseClicked(UIMouseEvent* event)
{
	return TRUE;
}
