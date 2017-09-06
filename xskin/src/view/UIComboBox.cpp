#include "stdafx.h"
#include "../../include/view/UIComboBox.h"
#include "../../include/view/UIListView.h"
#include "../../include/view/UIRootView.h"
#include "../../include/view/UIPanel.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/event/UIMouseEvent.h"

IMPLEMENT_XML_NODE(UIComboBox)
	
BEGIN_XML_PROPERTY(UIComboBox)
	SET_SKIN(dropdown:skin, m_pListView)
	SET_INT(dropdown:height, m_nDroplistHeight)
END_XML_PROPERTY()

UIComboBox::UIComboBox(UIView* parent)
 : UIControl(parent)
{
	m_rcPadding.SetRect(4, 0, 4, 0);
	m_alignment.cx = UIAlignment::ALIGN_LEFT;
	m_alignment.cy = UIAlignment::ALIGN_VCENTER;
	m_nDroplistHeight = -1;
	m_pListView = new UIListView;
	m_pListView->SetSizePolicy(UISizePolicy::FIXED, UISizePolicy::FIXED);
	m_pPanel = new UIPanel(this);
	m_pPanel->Create();
	m_pPanel->SetContentView(m_pListView);
}

UIComboBox::~UIComboBox()
{
	if(m_pPanel)
		delete m_pPanel;
}

void UIComboBox::SetModel(UIModel* model)
{
	m_pListView->SetModel(model);
}

UIModel* UIComboBox::GetModel() const
{
	return m_pListView->GetModel();
}

UIView* UIComboBox::GetItem(int nIndex) const
{
	return m_pListView->GetItem(nIndex);
}

void UIComboBox::SetCheckedIndex(int nIndex)
{
	m_pListView->SetCheckedIndex(nIndex);
}

void UIComboBox::SetCheckedItem(UIView* v)
{
	m_pListView->SetCheckedItem(v);
}

UIView* UIComboBox::GetCheckedItem() const
{
	return m_pListView->GetCheckedItem();
}

int UIComboBox::GetCheckedIndex() const
{
	return m_pListView->GetCheckedIndex();
}

void UIComboBox::SetDroplistHeight(int nHeight)
{
	m_nDroplistHeight = nHeight;
}

void UIComboBox::Activate(BOOL b)
{
	if(b)
	{
		UIRect rcTemp(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
		UIRect rcScreen = MapToScreen(this, rcTemp);
		m_nDroplistHeight = (m_nDroplistHeight == -1) ? m_nDroplistHeight = m_pListView->GetPreferredSize().cy : m_nDroplistHeight;
		m_pPanel->SetBounds(rcScreen.left, rcScreen.bottom, rcScreen.right, rcScreen.bottom + m_nDroplistHeight);
		m_pPanel->ShowNoActive();
		m_pListView->ScrollToTop();
		m_pListView->RequestFocus();
		SetChecked(TRUE);
	}
	else
	{
		m_pPanel->Hide();
		SetChecked(FALSE);
	}

	__super::Activate(b);
}

BOOL UIComboBox::IsFocusFrameVisible() const
{
	return FALSE;
}

BOOL UIComboBox::OnMouseWheel(UIWheelEvent* event)
{
	m_pPanel->ForwardWheelEvent(event);
	return TRUE;
}

BOOL UIComboBox::OnKeyPressed(UIKeyEvent* event)
{
	m_pPanel->ForwardKeyPressedEvent(event);
	return TRUE;
}

BOOL UIComboBox::OnKeyReleased(UIKeyEvent* event)
{
	m_pPanel->ForwardKeyReleasedEvent(event);
	return TRUE;
}

BOOL UIComboBox::OnFocusGain(UIFocusEvent* event)
{
	Activate(TRUE);
	return TRUE;
}

BOOL UIComboBox::OnFocusLost(UIFocusEvent* event)
{
	Activate(FALSE);
	return TRUE;
}

void UIComboBox::OnInit()
{
	SetCheckedIndex(0);
	__super::OnInit();
}

void UIComboBox::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	int bkState = GetBkState();
	if(bkState == BS_NORMAL)
	{
		if(IsChecked())
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
		else
			m_bkNormal.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	}
	else if(bkState == BS_HOT)
	{
		if(IsChecked())
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
		else
			m_bkHot.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	}
	else if(bkState == BS_PUSHED)
	{
		if(IsChecked())
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
		else
			m_bkPushed.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	}
	else if(bkState == BS_DISABLED)
	{
		m_bkDisabled.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	}

	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
	COLORREF clrOld = ::SetTextColor(hdc, m_bEnabled ? m_clrText : m_clrTextDisabled);
	rcPaint.right -= m_rcBounds.Height();
	rcPaint.left += m_rcPadding.left;
	rcPaint.right -= m_rcPadding.right;
	rcPaint.top += m_rcPadding.top;
	rcPaint.bottom -= m_rcPadding.bottom;
	UIRender::DrawString(hdc, rcPaint, m_szText, m_alignment);
	::SetTextColor(hdc, clrOld);
	::SelectObject(hdc, hOldFont);
}

BOOL UIComboBox::OnMousePressed(UIMouseEvent* event)
{
	if(event->GetSource() == this && event->GetFlags() & UIEvent::LEFT_BUTTON_MASK)
	{
		if(HasFocus())
			ClearFocus();
		else
			RequestFocus();
	}
	
	return TRUE;
}

BOOL UIComboBox::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetSource() == m_pListView && event->GetCommand() == ACTION_SELECT)
	{
		UIView* pSel = m_pListView->GetCheckedItem();
		UIControl* pItem = dynamic_cast<UIControl*>(pSel);
		if(pItem)
		{
			SetText(pItem->GetText());
			if(GetRootView())
				ClearFocus();

			return TRUE;
		}
	}

	return __super::OnActionPerformed(event);
}