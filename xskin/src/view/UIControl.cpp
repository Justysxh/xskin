#include "stdafx.h"
#include "../../include/view/UIControl.h"
#include "../../include/event/UIMouseEvent.h"
#include "../../include/event/UIKeyEvent.h"
#include "../../include/event/UIActionEvent.h"

IMPLEMENT_XML_NODE(UIControl)

BEGIN_XML_PROPERTY(UIControl)
	SET_STRING(text, m_szText)
	SET_COLOR(textColor, m_clrText)
	SET_ALIGN(textAlign, m_alignment)
	SET_FONT(font, m_pFont)
	SET_DRAWABLE(bkNormalImage, m_bkNormal)
	SET_DRAWABLE(bkHotImage, m_bkHot)
	SET_DRAWABLE(bkPushedImage, m_bkPushed)
	SET_DRAWABLE(bkDisabledImage, m_bkDisabled)
	SET_DRAWABLE(bkCheckedImage, m_bkChecked)
END_XML_PROPERTY()

UIControl::UIControl(UIView* parent)
 : UIView(parent)
{
	m_bkState = BS_NORMAL;
	m_clrText = RGB(0, 0, 0);
	m_clrTextDisabled = RGB(200, 200, 200);
	m_alignment.cx = UIAlignment::ALIGN_LEFT;
	m_alignment.cy = UIAlignment::ALIGN_VCENTER;
	m_szText = _T("");
	m_rcPadding.SetRect(2, 2, 2, 2);
	m_pFont = UIApplication::Instance()->GetAssetManager()->GetGuiFont();
}

UIControl::~UIControl()
{
	if(m_pFont)
		m_pFont->Release();
}

void UIControl::SetText(const UIString& szText)
{
	if(m_szText == szText)
		return;

	m_szText = szText;
	Invalidate();

	UIActionEvent event(this, GetID(), ACTION_VALUE_CHANGED, 0);
	UIApplication::SendEvent(&event);
}

UIString UIControl::GetText() const
{
	return m_szText;
}

void UIControl::SetFont(const UIFont* pFont)
{
	if(m_pFont)
		m_pFont->Release();

	m_pFont = pFont;
	Invalidate();
}

const UIFont* UIControl::GetFont() const
{
	return m_pFont;
}

void UIControl::SetTextColor(COLORREF clr)
{
	m_clrText = clr;
	Invalidate();
}

COLORREF UIControl::GetTextColor() const
{
	return m_clrText;
}

void UIControl::SetTextAlign(UIAlignment::AlignFlag cx, UIAlignment::AlignFlag cy)
{
	m_alignment.cx = cx;
	m_alignment.cy = cy;
}

UIAlignment UIControl::GetTextAlign() const
{
	return m_alignment;
}

UISize UIControl::GetPreferredSize() const
{
	UISize size = __super::GetPreferredSize();

	if(m_bkNormal.IsValid())
	{
		size.cx = m_bkNormal.Width();
		size.cy = m_bkNormal.Height();
	}
	else if(m_bkHot.IsValid())
	{
		size.cx = m_bkHot.Width();
		size.cy = m_bkHot.Height();
	}
	else if(m_bkPushed.IsValid())
	{
		size.cx = m_bkPushed.Width();
		size.cy = m_bkPushed.Height();
	}
	else if(m_bkDisabled.IsValid())
	{
		size.cx = m_bkDisabled.Width();
		size.cy = m_bkDisabled.Height();
	}

	UISize szText;
	if(!m_szText.IsEmpty())
	{
		szText.cx = m_pFont->GetStringWidth(m_szText) + m_rcPadding.left + m_rcPadding.right;
		szText.cy = m_pFont->GetHeight() + m_rcPadding.top + m_rcPadding.bottom;
	}

	size.cx = max(size.cx, szText.cx);
	size.cy = max(size.cy, szText.cy);

	return size;
}

void UIControl::SetBkNormal(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkNormal.SetBitmap(pBitmap);
	m_bkNormal.SetPos(rcPos);
}

void UIControl::SetBkHot(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkHot.SetBitmap(pBitmap);
	m_bkHot.SetPos(rcPos);
}

void UIControl::SetBkPushed(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkPushed.SetBitmap(pBitmap);
	m_bkPushed.SetPos(rcPos);
}

void UIControl::SetBkDisabled(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkDisabled.SetBitmap(pBitmap);
	m_bkDisabled.SetPos(rcPos);
}

void UIControl::SetBkChecked(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkChecked.SetBitmap(pBitmap);
	m_bkChecked.SetPos(rcPos);
}

UIDrawable* UIControl::GetBkNormal()
{
	return &m_bkNormal;
}

UIDrawable*	UIControl::GetBkHot()
{
	return &m_bkHot;
}

UIDrawable*	UIControl::GetBkPushed()
{
	return &m_bkPushed;
}

UIDrawable*	UIControl::GetBkDisabled()
{
	return &m_bkDisabled;
}

UIDrawable*	UIControl::GetBkChecked()
{
	return &m_bkChecked;
}

void UIControl::SetDisabledTextColor(COLORREF clr)
{
	m_clrTextDisabled = clr;
}

COLORREF UIControl::GetDisabledTextColor() const
{
	return m_clrTextDisabled;
}

void UIControl::SetBkState(UIControl::BackgroundState state)
{
	m_bkState = state;
	Invalidate();
}

UIControl::BackgroundState UIControl::GetBkState() const
{
	return m_bkState;
}

void UIControl::SetEnabled(BOOL b)
{
	SetBkState(b ? BS_NORMAL : BS_DISABLED);
	__super::SetEnabled(b);
}

void UIControl::OnInit()
{
	if(!m_bEnabled)
		m_bkState = BS_DISABLED;

	__super::OnInit();
}

BOOL UIControl::OnMousePressed(UIMouseEvent* event)
{
	if(event->GetFlags() & UIEvent::LEFT_BUTTON_MASK)
		SetBkState(BS_PUSHED);

	return TRUE;
}

BOOL UIControl::OnMouseReleased(UIMouseEvent* event)
{
	if(event->GetFlags() & UIEvent::LEFT_BUTTON_MASK)
	{
		UIPoint point = MapToView(this, event->GetLocation());
		if(HitTest(point))
			SetBkState(BS_HOT);
		else
			SetBkState(BS_NORMAL);
	}

	return TRUE;
}

BOOL UIControl::OnMouseEntered(UIMouseEvent* event)
{
	SetBkState(BS_HOT);

	return TRUE;
}

BOOL UIControl::OnMouseExited(UIMouseEvent* event)
{
	SetBkState(BS_NORMAL);

	return TRUE;
}

BOOL UIControl::OnKeyPressed(UIKeyEvent* event)
{
	if(event->GetKeyCode() == VK_SPACE)
	{
		SetBkState(BS_PUSHED);
	}
	else if(event->GetKeyCode() == VK_RETURN)
	{
		Activate(TRUE);
	}

	return TRUE;
}

BOOL UIControl::OnKeyReleased(UIKeyEvent* event)
{
	SetBkState(BS_NORMAL);
	if(event->GetKeyCode() == VK_SPACE)
	{
		Activate(TRUE);
	}

	return TRUE;
}