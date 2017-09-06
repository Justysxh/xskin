#include "stdafx.h"
#include "../../include/animation/UIBoundAnimation.h"
#include "../../include/view/UIView.h"

UIBoundAnimation::UIBoundAnimation(UIView* pTarget, const UIRect& rcStart, const UIRect& rcEnd)
	: UIAnimation(pTarget)
{
	m_rcStart = rcStart;
	m_rcEnd = rcEnd;
}

UIBoundAnimation::~UIBoundAnimation()
{

}

void UIBoundAnimation::SetStartValue(const UIRect& rcStart)
{
	m_rcStart = rcStart;
}

void UIBoundAnimation::SetEndValue(const UIRect& rcEnd)
{
	m_rcEnd = rcEnd;
}

void UIBoundAnimation::OnAnimationStarted()
{

}

void UIBoundAnimation::OnAnimationStopped()
{

}

void UIBoundAnimation::OnAnimationStepped()
{
	UIRect rcDst = GetValueBetween(m_rcStart, m_rcEnd);

	m_pTarget->Invalidate();
	m_pTarget->SetBounds(rcDst);
	m_pTarget->InvalidateLayout();
	m_pTarget->Invalidate();

	__super::OnAnimationStepped();
}