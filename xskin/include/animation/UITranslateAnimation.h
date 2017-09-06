#ifndef __UITRANSLATE_ANIMATION_H_INCLUDED__
#define __UITRANSLATE_ANIMATION_H_INCLUDED__

#include "UIAnimation.h"
#include "../../include/core/UIPoint.h"

class UIView;
class XSKIN_API UITranslateAnimation : public UIAnimation
{
public:
	UITranslateAnimation(UIView* pTarget = NULL,
		int nFromXDelta = 0, int nToXDelta = 0, int nFromYDelta = 0, int nToYDelta = 0);
	~UITranslateAnimation();

public:
	void	SetStartValue(int nFromXDelta, int nFromYDelta);
	void	SetEndValue(int nToXDelta, int nToYDelta);
	void	OnAnimationStarted() OVERRIDE;
	void	OnAnimationStepped() OVERRIDE;
	void	OnAnimationStopped() OVERRIDE;

private:
	int m_nFromXDelta;
	int m_nFromYDelta;
	int m_nToXDelta;
	int m_nToYDelta;
	UIPoint m_pos;
};
#endif //! __UITRANSLATE_ANIMATION_H_INCLUDED__