#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../../include/animation/UIAnimation.h"
#include "../../include/view/UIView.h"
#include "../../include/view/UIRootView.h"

UIAnimation::UIAnimation(UIView* pTarget)
{
	m_pTarget = pTarget;
	m_bAnimating = FALSE;
	m_bCycleFlip = FALSE;
	m_bStarted = FALSE;
	m_nRepeatCount = 0;
	m_nRepeated = 0;
	m_nStartTimeTick = 0;
	m_nDuration = 0;
	m_tweenType = EASE_OUT;
	m_repeatMode = RESTART;

	if(m_pTarget)
		m_pTarget->SetAnimation(this);
}

UIAnimation::~UIAnimation()
{

}

void UIAnimation::Start()
{
	if(!m_pTarget || !m_nDuration || m_bAnimating)
		return;

	m_nRepeated = 0;
	m_bCycleFlip = FALSE;
	m_nStartTimeTick = (m_nStartTimeTick == 0 ? ::GetTickCount() : m_nStartTimeTick);
	m_pTarget->GetRootView()->ScheduleAnimation(m_pTarget);
	m_pTarget->Invalidate();
	m_bAnimating = TRUE;
}

void UIAnimation::Stop()
{
	if(!m_bAnimating)
		return;

	m_bStarted = FALSE;
	m_bAnimating = FALSE;
	m_nStartTimeTick = 0;
	OnAnimationStopped();
}

void UIAnimation::Reset()
{
	if(m_bAnimating)
		return;

	m_bStarted = FALSE;
	m_bAnimating = FALSE;
	m_bCycleFlip = FALSE;
	m_nStartTimeTick = 0;
	m_nRepeated = 0;
	m_nDuration = 0;
	m_nRepeatCount = 0;
	m_tweenType = EASE_OUT;
	m_repeatMode = RESTART;
}

void UIAnimation::SetTarget(UIView* pTarget)
{
	m_pTarget = pTarget;
}

UIView* UIAnimation::GetTarget()
{
	return m_pTarget;
}

void UIAnimation::SetStartTime(DWORD dwStartTimeTick)
{
	m_nStartTimeTick = dwStartTimeTick;
}

DWORD UIAnimation::GetStartTime() const
{
	return m_nStartTimeTick;
}

BOOL UIAnimation::IsAnimating() const
{
	return m_bAnimating;
}

void UIAnimation::SetDuration(int nDuration)
{
	m_nDuration = nDuration;
}

int	UIAnimation::GetDuration() const
{
	return m_nDuration;
}

void UIAnimation::SetRepeatCount(int nCount)
{
	m_nRepeatCount = nCount;
}

int	UIAnimation::GetRepeatCount() const
{
	return m_nRepeatCount;
}

void UIAnimation::SetRepeatMode(RepeatMode mode)
{
	m_repeatMode = mode;
}

UIAnimation::RepeatMode UIAnimation::GetRepeatMode() const
{
	return m_repeatMode;
}

void UIAnimation::SetTweenType(TweenType type)
{
	m_tweenType = type;
}

UIAnimation::TweenType UIAnimation::GetTweenType() const
{
	return m_tweenType;
}

double UIAnimation::GetCurrentValue()
{
	double state = static_cast<double>(::GetTickCount() - m_nStartTimeTick) / m_nDuration;
	state = XCLAMP(state, 0, 1.0);

	if(m_bCycleFlip)
		state = 1.0 - state;

	switch(m_tweenType)
	{
	case EASE_IN:
	{
		state = pow(state, 2);
	}
	break;
	case EASE_OUT:
	{
		state = 1.0 - pow(1.0 - state, 2);
	}
	break;
	case EASE_IN_OUT:
	{
		state = state < 0.5 ? pow(state * 2, 2) / 2.0 : 1.0 - (pow((state - 1.0) * 2, 2) / 2.0);
	}
	break;
	case FAST_IN_OUT:
	{
		state = (pow(state - 0.5, 3) + 0.125) / 0.25;
	}
	break;
	case LINEAR:
	{

	}
	break;
	case EASE_OUT_SNAP:
	{
		state = 0.95 * (1.0 - pow(1.0 - state, 2));
	}
	break;
	case ELASTIC_EASE_OUT:
	{
		state = pow(2, -10 * state) * sin((state - 0.1) * 5 * M_PI) + 1;
	}
	break;
	case BOUNCE_EASE_OUT:
	{
		if(state < (1 / 2.75))
			state = 7.5625 * state * state;

		else if(state < (2 / 2.75))
			state = 7.5625 * (state -= (1.5 / 2.75)) * state + 0.75;

		else if(state < (2.5 / 2.75))
			state = 7.5625 * (state -= (2.25 / 2.75)) * state + 0.9375;

		else
			state = 7.5625 * (state -= (2.625 / 2.75)) * state + 0.984375;
	}	
	break;
	case ZERO:
		state = 0;
	}

	return state;
}

double	UIAnimation::GetValueBetween(double start, double target)
{
	return start + (target - start) * GetCurrentValue();
}

int UIAnimation::GetValueBetween(int start, int target)
{
	if(start == target)
	{
		return start;
	}

	double delta = static_cast<double>(target - start);
	if(delta < 0)
	{
		delta--;
	}
	else
	{
		delta++;
	}
	
	return start + static_cast<int>(GetCurrentValue() * _nextafter(delta, 0));
}

UIRect UIAnimation::GetValueBetween(const UIRect start, const UIRect& target)
{
	return UIRect(GetValueBetween(start.left, target.left),
		GetValueBetween(start.top, target.top),
		GetValueBetween(start.right, target.right),
		GetValueBetween(start.bottom, target.bottom));
}

COLORREF UIAnimation::GetValueBetween(COLORREF start, COLORREF target)
{
	return RGB(0, 0, 0);
}

void UIAnimation::OnAnimationStarted()
{

}

void UIAnimation::OnAnimationStepped()
{
	if(!m_bStarted)
	{
		m_bStarted = TRUE;
		m_pTarget->SetVisible(TRUE);
		OnAnimationStarted();
	}
	double value = GetCurrentValue();
	if(::GetTickCount() - m_nStartTimeTick >= m_nDuration)
	{
		if(m_nRepeated == m_nRepeatCount && m_nRepeatCount != INFINITE)
		{
			Stop();
		}
		else
		{
			if(m_nRepeatCount > 0)
				m_nRepeated++;

			if(m_repeatMode == REVERSE)
				m_bCycleFlip = m_bCycleFlip ? FALSE : TRUE;

			m_nStartTimeTick = ::GetTickCount();
		}
	}
}

void UIAnimation::OnAnimationStopped()
{

}
