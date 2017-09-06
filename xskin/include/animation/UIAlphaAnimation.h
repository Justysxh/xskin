#ifndef __UIALPHA_ANIMATION_H_INCLUDED__
#define __UIALPHA_ANIMATION_H_INCLUDED__

#include "UIAnimation.h"

class UIView;
class XSKIN_API UIAlphaAnimation : public UIAnimation
{
public:
			UIAlphaAnimation(UIView* pTarget = NULL, int nStart = 0, int nEnd = 0);
			~UIAlphaAnimation();

public:
	void	SetStartValue(int nStart);
	void	SetEndValue(int nEnd);
	void	OnAnimationStarted() OVERRIDE;
	void	OnAnimationStepped() OVERRIDE;
	void	OnAnimationStopped() OVERRIDE;

private:
	int m_nStart;
	int m_nEnd;
};
#endif //! __UIALPHA_ANIMATION_H_INCLUDED__