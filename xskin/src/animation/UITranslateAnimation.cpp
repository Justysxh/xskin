#include "stdafx.h"
#include "../../include/animation/UITranslateAnimation.h"
#include "../../include/view/UIView.h"

UITranslateAnimation::UITranslateAnimation(UIView* pTarget,
	int nFromXDelta, int nToXDelta, int nFromYDelta, int nToYDelta)
	: UIAnimation(pTarget)
{
	m_nFromXDelta = nFromXDelta;
	m_nFromYDelta = nFromYDelta;
	m_nToXDelta = nToXDelta;
	m_nToYDelta = nToYDelta;
}

UITranslateAnimation::~UITranslateAnimation()
{

}

void UITranslateAnimation::SetStartValue(int nFromXDelta, int nFromYDelta)
{
	m_nFromXDelta = nFromXDelta;
	m_nFromYDelta = nFromYDelta;
}

void UITranslateAnimation::SetEndValue(int nToXDelta, int nToYDelta)
{
	m_nToXDelta = nToXDelta;
	m_nToYDelta = nToYDelta;
}

void UITranslateAnimation::OnAnimationStarted()
{
	m_pos = m_pTarget->GetPosition();
}

void UITranslateAnimation::OnAnimationStepped()
{
	__super::OnAnimationStepped();

	int x = GetValueBetween(m_pos.x + m_nFromXDelta, m_pos.x + m_nToXDelta);
	int y = GetValueBetween(m_pos.y + m_nFromYDelta, m_pos.y + m_nToYDelta);
	m_pTarget->Invalidate();
	m_pTarget->SetPosition(x, y);
	m_pTarget->Invalidate();
}

void UITranslateAnimation::OnAnimationStopped()
{

}