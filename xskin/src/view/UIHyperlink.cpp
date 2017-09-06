#include "stdafx.h"
#include "../../include/view/UIHyperlink.h"
#include "../../include/view/UIWindow.h"
#include "../../include/view/UIRootView.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/gfx/UIPen.h"

IMPLEMENT_XML_NODE(UIHyperlink)

UIHyperlink::UIHyperlink(UIView* parent)
 : UIButton(parent)
{
	m_hCursor = UIApplication::Instance()->GetAssetManager()->GetCursor(UIAsset::CURSOR_HAND);
}

UIHyperlink::~UIHyperlink()
{

}

UISize UIHyperlink::GetPreferredSize() const
{
	UISize size;
	size.cx = m_pFont->GetStringWidth(m_szText) + m_rcPadding.left + m_rcPadding.right;
	size.cy = m_pFont->GetHeight() + m_rcPadding.top + m_rcPadding.bottom;
	return size;
}

void UIHyperlink::OnPaint(HDC hdc)
{
	UIRect rcText(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcText, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
	COLORREF clrOld = ::SetTextColor(hdc, m_bEnabled ? m_clrText : m_clrTextDisabled);
	rcText.left += m_rcPadding.left;
	rcText.right -= m_rcPadding.right;
	rcText.top += m_rcPadding.top;
	rcText.bottom -= m_rcPadding.bottom;
	UIRender::DrawString(hdc, rcText, m_szText, m_alignment);
	::SetTextColor(hdc, clrOld);
	::SelectObject(hdc, hOldFont);
}