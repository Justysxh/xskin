#include "stdafx.h"
#include "../../include/view/UISlider.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/event/UIMouseEvent.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/event/UIKeyEvent.h"

UISlider::UISliderButton::UISliderButton(UIView* parent)
 : UIControl(parent)
{

}

UISlider::UISliderButton::~UISliderButton()
{

}

void UISlider::UISliderButton::OnPaint(HDC hdc)
{
	UIDrawable::FillMode fm = UIDrawable::FILL_NONE;
	if(GetParent())
	{
		UISlider* pSlider = static_cast<UISlider*>(GetParent());
		fm = (pSlider->GetDirection() == HORIZONTAL) ? UIDrawable::FILL_REPEAT_X : UIDrawable::FILL_REPEAT_Y;
	}

	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	int bkState = GetBkState();
	if(bkState == BS_NORMAL)
		m_bkNormal.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	else if(bkState == BS_HOT)
		m_bkHot.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	else if(bkState == BS_PUSHED)
		m_bkPushed.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
	else if(bkState == BS_DISABLED)
		m_bkDisabled.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_X, m_nOpacity);
}

BOOL UISlider::UISliderButton::OnMousePressed(UIMouseEvent* event)
{
	__super::OnMousePressed(event);

	return FALSE;
}

BOOL UISlider::UISliderButton::OnMouseDragged(UIMouseEvent* event)
{
	__super::OnMouseDragged(event);

	return FALSE;
}

IMPLEMENT_XML_NODE(UISlider)

BEGIN_XML_PROPERTY(UISlider)
	SET_SKIN(handle:skin, m_pHandle)
END_XML_PROPERTY()

UISlider::UISlider(UIView* parent)
 : UIProgressBar(parent)
{
	m_bFocusable = TRUE;
	m_nStepUnit = DEFAULT_STEP_UNIT;
	m_pHandle = new UISliderButton(this);
	m_pHandle->SetSizePolicy(UISizePolicy::PREFERRED, UISizePolicy::PREFERRED);
}

UISlider::~UISlider()
{

}

void UISlider::UpdateTrackPos()
{
	int nMove;
	UIRect rcTrack;
	UISize szTrack = m_pHandle->GetPreferredSize();
	if(m_direction == HORIZONTAL)
	{
		nMove = m_rcBounds.Width() - m_pHandle->GetPreferredSize().cx;
		rcTrack.left = static_cast<int>(static_cast<double>(m_nValue - m_szRange.cx) / (m_szRange.cy - m_szRange.cx) * nMove);
		rcTrack.right = rcTrack.left + szTrack.cx;
		rcTrack.top = 0;
		rcTrack.bottom = m_rcBounds.Height();
	}
	else
	{
		nMove = m_rcBounds.Height() - szTrack.cy;
		rcTrack.left = 0;
		rcTrack.right = m_rcBounds.Width();
		rcTrack.bottom = m_rcBounds.Height()
			- static_cast<int>(static_cast<double>(m_nValue - m_szRange.cx) / (m_szRange.cy - m_szRange.cx) * nMove);
		rcTrack.top = rcTrack.bottom - szTrack.cy;
	}

	m_pHandle->SetBounds(rcTrack);
}

void UISlider::SetStepUnit(int nUnit)
{
	m_nStepUnit = nUnit;
}

int UISlider::GetStepUnit() const
{
	return m_nStepUnit;
}

void UISlider::StepForward()
{
	int nValue = Clamp(m_nValue + m_nStepUnit, m_szRange.cx, m_szRange.cy);
	SetValue(nValue);
}

void UISlider::SetEnabled(BOOL b)
{
	m_pHandle->SetEnabled(b);
	__super::SetEnabled(b);
}

void UISlider::StepBackward()
{
	int nValue = Clamp(m_nValue - m_nStepUnit, m_szRange.cx, m_szRange.cy);
	SetValue(nValue);
}

void UISlider::SetTrackPos(const UIRect& rcTrack)
{
	UISize size = GetSize();
	UISize szTrack = m_pHandle->GetPreferredSize();
	int nMove = (m_direction == HORIZONTAL) ? size.cx  - szTrack.cx : size.cy - szTrack.cy;

	UIRect bound;
	double ratio = 0.0;
	if(m_direction == HORIZONTAL)
	{
		bound.top = 0;
		bound.bottom = size.cy;
		bound.left = Clamp(rcTrack.left, 0, nMove);
		bound.right = bound.left + szTrack.cx;
		ratio = bound.left / static_cast<double>(nMove);
	}
	else
	{
		bound.left = 0;
		bound.right = size.cx;
		bound.bottom = Clamp(rcTrack.bottom, szTrack.cy, size.cy);
		bound.top = bound.bottom - szTrack.cy;
		ratio = (size.cy - bound.bottom) / static_cast<double>(nMove);
	}

	m_pHandle->SetBounds(bound);
	int nValue = m_szRange.cx + static_cast<int>((m_szRange.cy - m_szRange.cx) * ratio);
	SetValue(nValue);
}

UISize UISlider::GetPreferredSize() const
{
	UISize size = __super::GetPreferredSize();
	UIRect bound(0, 0, size.cx, size.cy);
	UIRect temp;
	if(m_pHandle)
	{
		UISize szPref = m_pHandle->GetPreferredSize();
		temp.SetRect(0, 0, szPref.cx, szPref.cy);
		bound.UnionRect(bound, temp);
	}

	return bound.GetSize();
}

void UISlider::OnLayout()
{
	UpdateTrackPos();

	__super::OnLayout();
}

void UISlider::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	UIDrawable::FillMode fm;
	UIRect rcFragment;
	UIRect rcTrack = m_pHandle->GetBounds();
	if(m_direction == HORIZONTAL)
	{
		rcFragment.left = 0;
		rcFragment.right = rcTrack.left;
		rcFragment.top = 0;
		rcFragment.bottom = rcPaint.bottom;
		fm = UIDrawable::FILL_REPEAT_X;
	}
	else
	{
		rcFragment.left = 0;
		rcFragment.right = rcPaint.right;
		rcFragment.bottom = rcPaint.bottom;
		rcFragment.top = rcTrack.bottom;
		fm = UIDrawable::FILL_REPEAT_Y;
	}

	m_bkgnd.Draw(hdc, rcPaint, fm, m_nOpacity);
	m_bkFragment.Draw(hdc, rcFragment, fm, m_nOpacity);
}

void UISlider::OnInit()
{
	m_pHandle->SetEnabled(m_bEnabled);
}

BOOL UISlider::OnMousePressed(UIMouseEvent* event)
{
	if(event->GetSource() == m_pHandle)
		m_ptLast = event->GetLocation();

	return TRUE;
}

BOOL UISlider::OnMouseDragged(UIMouseEvent* event)
{
	if(event->GetSource() == m_pHandle)
	{
		UIPoint point = event->GetLocation();
		int nOffset = (m_direction == HORIZONTAL) ? point.x - m_ptLast.x : point.y - m_ptLast.y;
		UIRect rcTrack = m_pHandle->GetBounds();
		UIPoint ptTrack(rcTrack.left, rcTrack.top);

		if(m_direction == HORIZONTAL)
			rcTrack.OffsetRect(nOffset, 0);
		else
			rcTrack.OffsetRect(0, nOffset);

		SetTrackPos(rcTrack);

		UIPoint ptOffset = m_pHandle->GetPosition() - ptTrack;
		m_ptLast.Offset(ptOffset.x, ptOffset.y);
	}

	return TRUE;
}

BOOL UISlider::OnKeyPressed(UIKeyEvent* event)
{
	if(m_direction == HORIZONTAL)
	{
		if(event->GetKeyCode() == VK_LEFT)
			StepBackward();
		else if(event->GetKeyCode() == VK_RIGHT)
			StepForward();
	}
	else
	{
		if(event->GetKeyCode() == VK_UP)
			StepForward();
		else if(event->GetKeyCode() == VK_DOWN)
			StepBackward();
	}
	
	return TRUE;
}

BOOL UISlider::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetCommand() == ACTION_VALUE_CHANGED && m_pHandle->GetBkState() != BS_PUSHED)
	{
		UpdateTrackPos();
	}

	return FALSE;
}