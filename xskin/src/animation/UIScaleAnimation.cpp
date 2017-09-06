#include "stdafx.h"
#include "../../include/animation/UIScaleAnimation.h"
#include "../../include/view/UIView.h"

UIScaleAnimation::UIScaleAnimation(UIView* pTarget,
	double fromXScale, double toXScale, double fromYScale, double toYScale)
	: UIAnimation(pTarget)
{
	m_rcBound.SetRectEmpty();
	m_anchor = ANCHOR_CENTER;
	m_dFromXScale = fromXScale;
	m_dFromYScale = fromYScale;
	m_dToXScale = toXScale;
	m_dToYScale = toYScale;
}

UIScaleAnimation::~UIScaleAnimation()
{

}

void UIScaleAnimation::SetStartValue(double fromXScale, int fromYScale)
{
	m_dFromXScale = fromXScale;
	m_dFromYScale = fromYScale;
}

void UIScaleAnimation::SetEndValue(double toXScale, double toYScale)
{
	m_dToXScale = toXScale;
	m_dToYScale = toYScale;
}

void UIScaleAnimation::SetScaleAnchor(ScaleAnchor anchor)
{
	m_anchor = anchor;
}

void UIScaleAnimation::OnAnimationStarted()
{
	m_rcBound = m_pTarget->GetBounds();
}

void UIScaleAnimation::OnAnimationStepped()
{
	__super::OnAnimationStepped();

	double xScale = GetValueBetween(m_dFromXScale, m_dToXScale);
	double yScale = GetValueBetween(m_dFromYScale, m_dToYScale);
	int width = static_cast<int>(m_rcBound.Width() * xScale);
	int height = static_cast<int>(m_rcBound.Height() * yScale);
	m_pTarget->Invalidate();

	UIRect rcBound;
	if(m_anchor == ScaleAnchor::ANCHOR_CENTER)
	{
		rcBound.left = m_rcBound.left + (m_rcBound.Width() - width) / 2;
		rcBound.right = rcBound.left + width;
		rcBound.top = m_rcBound.top + (m_rcBound.Height() - height) / 2;
		rcBound.bottom = rcBound.top + height;
	}
	else if(m_anchor == ScaleAnchor::ANCHOR_TOPLEFT)
	{
		rcBound.left = m_rcBound.left;
		rcBound.right = rcBound.left + width;
		rcBound.top = m_rcBound.top;
		rcBound.bottom = rcBound.top + height;
	}
	else if(m_anchor == ScaleAnchor::ANCHOR_TOPRIGHT)
	{
		rcBound.right = m_rcBound.right;
		rcBound.left = rcBound.right - width;
		rcBound.top = m_rcBound.top;
		rcBound.bottom = rcBound.top + height;
	}
	else if(m_anchor == ScaleAnchor::ANCHOR_BOTTOM_LEFT)
	{
		rcBound.left = m_rcBound.left;
		rcBound.right = rcBound.left + width;
		rcBound.bottom = m_rcBound.bottom;
		rcBound.top = rcBound.bottom - height;
	}
	else if(m_anchor == ScaleAnchor::ANCHOR_BOTTOM_RIGHT)
	{
		rcBound.right = m_rcBound.right;
		rcBound.left = rcBound.right - width;
		rcBound.bottom = m_rcBound.bottom;
		rcBound.top = rcBound.bottom - height;
	}

	m_pTarget->SetBounds(rcBound);
	m_pTarget->Invalidate();
}

void UIScaleAnimation::OnAnimationStopped()
{

}