#include "stdafx.h"
#include "../../include/view/UIImageButton.h"
#include "../../include/gfx/UIRender.h"

IMPLEMENT_XML_NODE(UIImageButton)
	
BEGIN_XML_PROPERTY(UIImageButton)
	SET_DRAWABLE(icon, m_bkIcon)
	SET_INT(spacing, m_nSpacing)
END_XML_PROPERTY()

UIImageButton::UIImageButton(UIView* parent)
 : UIButton(parent)
{
	m_alignment.cx = UIAlignment::ALIGN_RIGHT;
	m_alignment.cy = UIAlignment::ALIGN_BOTTOM;
	m_nSpacing = BITMAP_TEXT_SPACING;
	m_rcPadding.SetRect(2, 2, 2, 2);
}

UIImageButton::~UIImageButton()
{

}

void UIImageButton::SetIcon(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkIcon.SetBitmap(pBitmap);
	m_bkIcon.SetPos(rcPos);
	Invalidate();
}

void UIImageButton::SetSpacing(int nSpacing)
{
	m_nSpacing = nSpacing;
	Invalidate();
}

UISize UIImageButton::GetPreferredSize() const
{
	UIRect rcBound(0, 0, m_bkIcon.Width(), m_bkIcon.Height());
	int nOffset = (m_alignment.cy == UIAlignment::ALIGN_BOTTOM) ? m_bkIcon.Height() + m_nSpacing : m_bkIcon.Width() + m_nSpacing;
	UIRect rcText;
	rcText.left = (m_alignment.cy == UIAlignment::ALIGN_BOTTOM) ? 0 : nOffset;
	rcText.top = (m_alignment.cy == UIAlignment::ALIGN_BOTTOM) ? nOffset : 0;
	rcText.right = rcText.left + m_pFont->GetStringWidth(m_szText);
	rcText.bottom = rcText.top + m_pFont->GetHeight();
	rcBound.UnionRect(rcBound, rcText);
	rcBound.left -= m_rcPadding.left;
	rcBound.right += m_rcPadding.right;
	rcBound.top -= m_rcPadding.top;
	rcBound.bottom += m_rcPadding.bottom;

	return rcBound.GetSize();
}

void UIImageButton::OnPaint(HDC hdc)
{

	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	int bkState = GetBkState();
	if(bkState == BS_NORMAL)
	{
		if(IsChecked())
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
	}
	else if(bkState == BS_DISABLED)
	{
		m_bkDisabled.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}

	UIRect rcTemp;
	UIRect rcIcon(0, 0, m_bkIcon.Width(), m_bkIcon.Height());
	UIRect rcText;
	int nOffset;
	if(m_alignment.cy == UIAlignment::ALIGN_BOTTOM)
	{
		nOffset = rcIcon.Height() + m_nSpacing;
		rcText.SetRect(0, nOffset, m_pFont->GetStringWidth(m_szText), nOffset + m_pFont->GetHeight());
	}
	else
	{
		nOffset = rcIcon.Width() + m_nSpacing;
		rcText.SetRect(nOffset, 0, nOffset + m_pFont->GetStringWidth(m_szText), m_pFont->GetHeight());
	}
	rcTemp.UnionRect(rcIcon, rcText);

	// center the union box.
	UIRect rcCenter;
	rcCenter.left = (rcPaint.Width() - rcTemp.Width()) / 2;
	rcCenter.top = (rcPaint.Height() - rcTemp.Height()) / 2;
	rcCenter.right = rcCenter.left + rcTemp.Width();
	rcCenter.bottom = rcCenter.top + rcTemp.Height();

	// draw icon.
	rcPaint.left = (m_alignment.cy == UIAlignment::ALIGN_BOTTOM) ? rcCenter.left + (rcCenter.Width() - rcIcon.Width()) / 2 : rcCenter.left;
	rcPaint.right = rcPaint.left + rcIcon.Width();
	rcPaint.top = (m_alignment.cy == UIAlignment::ALIGN_BOTTOM) ? rcCenter.top : rcCenter.top + (rcCenter.Height() - rcIcon.Height()) / 2;
	rcPaint.bottom = rcPaint.top + rcIcon.Height();

	if(bkState == BS_PUSHED)
		rcPaint.OffsetRect(1, 1);

	m_bkIcon.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);

	// draw text.
	rcPaint.left = (m_alignment.cy == UIAlignment::ALIGN_BOTTOM) ? rcCenter.left + (rcCenter.Width() - rcText.Width()) / 2 : rcCenter.left + m_bkIcon.Width() + m_nSpacing;
	rcPaint.right = rcPaint.left + rcText.Width();
	rcPaint.top = (m_alignment.cy == UIAlignment::ALIGN_BOTTOM) ? rcCenter.bottom - rcText.Height() : rcCenter.top + (rcCenter.Height() - rcText.Height()) / 2;
	rcPaint.bottom = rcPaint.top + rcText.Height();

	if(bkState == BS_PUSHED)
		rcPaint.OffsetRect(1, 1);

	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
	COLORREF clrOld = ::SetTextColor(hdc, m_bEnabled ? m_clrText : m_clrTextDisabled);
	UIRender::DrawString(hdc, rcPaint, m_szText, m_alignment);
	::SetTextColor(hdc, clrOld);
	::SelectObject(hdc, hOldFont);
}