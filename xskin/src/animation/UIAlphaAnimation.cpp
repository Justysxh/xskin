#include "stdafx.h"
#include "../../include/view/UIView.h"
#include "../../include/animation/UIAlphaAnimation.h"

UIAlphaAnimation::UIAlphaAnimation(UIView* pTarget, int nStart, int nEnd)
 : UIAnimation(pTarget)
{
	m_nStart = nStart;
	m_nEnd = nEnd;
}

UIAlphaAnimation::~UIAlphaAnimation()
{

}

void UIAlphaAnimation::SetStartValue(int nStart)
{
	m_nStart = nStart;
}

void UIAlphaAnimation::SetEndValue(int nEnd)
{
	m_nEnd = nEnd;
}

void UIAlphaAnimation::OnAnimationStarted()
{

}

void UIAlphaAnimation::OnAnimationStopped()
{

}

void UIAlphaAnimation::OnAnimationStepped()
{
	int nAlpha = GetValueBetween(m_nStart, m_nEnd);
	m_pTarget->SetOpacity(nAlpha);
	__super::OnAnimationStepped();
}