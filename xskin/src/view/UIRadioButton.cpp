#include "stdafx.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/core/UIApplication.h"
#include "../../include/view/UIRadioButton.h"
#include "../../include/view/UIRootView.h"
#include "../../include/event/UIActionEvent.h"

IMPLEMENT_XML_NODE(UIRadioButton)

BEGIN_XML_PROPERTY(UIRadioButton)
	SET_DRAWABLE(bkCheckedNormalImage, m_bkCheckedNormal)
	SET_DRAWABLE(bkCheckedHotImage, m_bkCheckedHot)
	SET_DRAWABLE(bkCheckedPushedImage, m_bkCheckedPushed)
	SET_DRAWABLE(bkCheckedDisabledImage, m_bkCheckedDisabled)
END_XML_PROPERTY()

UIRadioButton::UIRadioButton(UIView* parent)
 : UIButton(parent)
{
	m_nSpacing = BITMAP_TEXT_SPACING;
}

UIRadioButton::~UIRadioButton()
{

}

void UIRadioButton::SetSpacing(int nSpacing)
{
	m_nSpacing = nSpacing;
}

void UIRadioButton::Activate(BOOL b)
{
	SetChecked(b);

	__super::Activate(b);
}

void UIRadioButton::SetChecked(BOOL b)
{
	if(b && !m_bChecked)
	{
		m_bChecked = TRUE;
		UIRootView* pRoot = GetRootView();
		if(pRoot)
		{
			std::vector<UIView*> views;
			pRoot->GetViewsWithGroup(m_szGroup, &views);
			std::vector<UIView*>::iterator itor;
			for(itor = views.begin(); itor != views.end(); ++itor)
			{
				if((*itor) != this)
				{
					UIRadioButton* v = dynamic_cast<UIRadioButton*>(*itor);
					if(v)
					{
						if(v->m_bChecked)
						{
							v->m_bChecked = FALSE;
							v->Invalidate();

							UIActionEvent event(v, v->GetID(), ACTION_CHECK, FALSE);
							UIApplication::SendEvent(&event);
						}
					}
				}
			}
		}

		Invalidate();
		UIActionEvent event(this, GetID(), ACTION_CHECK, TRUE);
		UIApplication::SendEvent(&event);
	}
}


void UIRadioButton::SetCheckedBkNormal(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkCheckedNormal.SetBitmap(pBitmap);
	m_bkCheckedNormal.SetPos(rcPos);
}

void UIRadioButton::SetCheckedBkHot(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkCheckedHot.SetBitmap(pBitmap);
	m_bkCheckedHot.SetPos(rcPos);
}

void UIRadioButton::SetCheckedBkPushed(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkCheckedPushed.SetBitmap(pBitmap);
	m_bkCheckedPushed.SetPos(rcPos);
}

void UIRadioButton::SetCheckedBkDisabled(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkCheckedDisabled.SetBitmap(pBitmap);
	m_bkCheckedDisabled.SetPos(rcPos);
}

BOOL UIRadioButton::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetCommand() == ACTION_CLICK)
	{
		SetChecked(TRUE);
		return TRUE;
	}
	else
	{
		return __super::OnActionPerformed(event);
	}
}

UISize UIRadioButton::GetPreferredSize() const
{
	UIRect rcBitmap;
	if(m_bkNormal.IsValid())
	{
		rcBitmap.SetWidth(m_bkNormal.Width());
		rcBitmap.SetHeight(m_bkNormal.Height());
	}
	else if(m_bkHot.IsValid())
	{
		rcBitmap.SetWidth(m_bkHot.Width());
		rcBitmap.SetHeight(m_bkHot.Height());
	}
	else if(m_bkPushed.IsValid())
	{
		rcBitmap.SetWidth(m_bkPushed.Width());
		rcBitmap.SetHeight(m_bkPushed.Height());
	}
	else if(m_bkDisabled.IsValid())
	{
		rcBitmap.SetWidth(m_bkDisabled.Width());
		rcBitmap.SetHeight(m_bkDisabled.Height());
	}
	else if(m_bkCheckedNormal.IsValid())
	{
		rcBitmap.SetWidth(m_bkCheckedNormal.Width());
		rcBitmap.SetHeight(m_bkCheckedNormal.Height());
	}
	else if(m_bkCheckedHot.IsValid())
	{
		rcBitmap.SetWidth(m_bkCheckedHot.Width());
		rcBitmap.SetHeight(m_bkCheckedHot.Height());
	}
	else if(m_bkCheckedPushed.IsValid())
	{
		rcBitmap.SetWidth(m_bkCheckedPushed.Width());
		rcBitmap.SetHeight(m_bkCheckedPushed.Height());
	}
	if(m_bkCheckedDisabled.IsValid())
	{
		rcBitmap.SetWidth(m_bkCheckedDisabled.Width());
		rcBitmap.SetHeight(m_bkCheckedDisabled.Height());
	}

	UIRect rcText(rcBitmap.right + m_nSpacing, 0,
		rcBitmap.right + m_nSpacing + m_pFont->GetStringWidth(m_szText), m_pFont->GetHeight());

	rcText.UnionRect(rcBitmap, rcText);
	rcText.left -= m_rcPadding.left;
	rcText.right += m_rcPadding.right;
	rcText.top -= m_rcPadding.top;
	rcText.bottom += m_rcPadding.bottom;
	return UISize(rcText.Width(), rcText.Height());
}

void UIRadioButton::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	UIRect rcBitmap(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	rcBitmap.left += m_rcPadding.left;
	rcBitmap.right -= m_rcPadding.right;
	rcBitmap.top += m_rcPadding.top;
	rcBitmap.bottom -= m_rcPadding.bottom;

	if(m_bkNormal.IsValid())
	{
		rcBitmap.SetWidth(m_bkNormal.Width());
		rcBitmap.SetHeight(m_bkNormal.Height());
	}
	else if(m_bkHot.IsValid())
	{
		rcBitmap.SetWidth(m_bkHot.Width());
		rcBitmap.SetHeight(m_bkHot.Height());
	}
	else if(m_bkPushed.IsValid())
	{
		rcBitmap.SetWidth(m_bkPushed.Width());
		rcBitmap.SetHeight(m_bkPushed.Height());
	}
	else if(m_bkDisabled.IsValid())
	{
		rcBitmap.SetWidth(m_bkDisabled.Width());
		rcBitmap.SetHeight(m_bkDisabled.Height());
	}

	rcBitmap.SetPos(m_rcPadding.left, (m_rcBounds.Height() - rcBitmap.Height()) / 2);

	BackgroundState bkState = GetBkState();
	if(bkState == BS_NORMAL)
	{
		if(m_bChecked && m_bkCheckedNormal.IsValid())
			m_bkCheckedNormal.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
		else
			m_bkNormal.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(bkState == BS_HOT)
	{
		if(m_bChecked && m_bkCheckedHot.IsValid())
			m_bkCheckedHot.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
		else
			m_bkHot.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(bkState == BS_PUSHED)
	{
		if(m_bChecked && m_bkCheckedPushed.IsValid())
			m_bkCheckedPushed.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
		else
			m_bkPushed.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(bkState == BS_DISABLED)
	{
		if(m_bChecked && m_bkCheckedDisabled.IsValid())
			m_bkCheckedDisabled.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
		else
			m_bkDisabled.Draw(hdc, rcBitmap, UIDrawable::FILL_NONE, m_nOpacity);
	}

	UIRect rcText(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	rcText.left = rcBitmap.right + m_nSpacing;
	rcText.right -= m_rcPadding.right;
	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
	COLORREF clrOld = ::SetTextColor(hdc, m_bEnabled ? m_clrText : m_clrTextDisabled);
	UIRender::DrawString(hdc, rcText, m_szText, m_alignment);
	::SetTextColor(hdc, clrOld);
	::SelectObject(hdc, hOldFont);
}