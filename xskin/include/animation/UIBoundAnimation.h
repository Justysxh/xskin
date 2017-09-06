#ifndef __UIBOUND_ANIMATION_H_INCLUDED__
#define __UIBOUND_ANIMATION_H_INCLUDED__

#include "UIAnimation.h"
#include "../core/UIRect.h"

class XSKIN_API UIBoundAnimation : public UIAnimation
{
public:
	UIBoundAnimation(UIView* pTarget = NULL, const UIRect& rcStart = UIRect(), const UIRect& rcEnd = UIRect());
	~UIBoundAnimation();

public:
	void	SetStartValue(const UIRect& rcStart);
	void	SetEndValue(const UIRect& rcEnd);

	void	OnAnimationStarted() OVERRIDE;
	void	OnAnimationStepped() OVERRIDE;
	void	OnAnimationStopped() OVERRIDE;

private:
	UIRect m_rcStart;
	UIRect m_rcEnd;
};
#endif //! __UIBOUND_ANIMATION_H_INCLUDED__