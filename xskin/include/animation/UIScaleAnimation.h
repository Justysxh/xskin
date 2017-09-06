#ifndef __UISCALE_ANIMATION_H_INCLUDED__
#define __UISCALE_ANIMATION_H_INCLUDED__

#include "UIAnimation.h"

class XSKIN_API UIScaleAnimation : public UIAnimation
{
public:
			UIScaleAnimation(UIView* pTarget = NULL,
				double fromXScale = 0, double toXScale = 0,
				double fromYScale = 0, double toYScale = 0);
			~UIScaleAnimation();

	enum ScaleAnchor
	{
		ANCHOR_CENTER = 0,
		ANCHOR_TOPLEFT,
		ANCHOR_TOPRIGHT,
		ANCHOR_BOTTOM_LEFT,
		ANCHOR_BOTTOM_RIGHT
	};

public:
	void	SetStartValue(double fromXScale, int fromYScale);
	void	SetEndValue(double toXScale, double toYScale);
	void	SetScaleAnchor(ScaleAnchor anchor);
	void	OnAnimationStarted() OVERRIDE;
	void	OnAnimationStepped() OVERRIDE;
	void	OnAnimationStopped() OVERRIDE;

private:
	UIRect m_rcBound;
	ScaleAnchor m_anchor;
	double m_dFromXScale;
	double m_dToXScale;
	double m_dFromYScale;
	double m_dToYScale;
};
#endif //! __UISCALE_ANIMATION_H_INCLUDED__