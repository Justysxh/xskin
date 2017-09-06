#ifndef __UIANIMATION_H_INCLUDED__
#define __UIANIMATION_H_INCLUDED__

#include "../core/UIObject.h"
#include "../core/UIRect.h"

class UIView;
class XSKIN_API UIAnimation : public UIObject
{
public:
	enum TweenType
	{
		LINEAR = 0,			// Linear.
		EASE_OUT,			// Fast in, slow out (default).
		EASE_IN,			// Slow in, fast out.
		EASE_IN_OUT,		// Slow in and out, fast in the middle.
		FAST_IN_OUT,		// Fast in and out, slow in the middle.
		EASE_OUT_SNAP,		// Fast in, slow out, snap to final value.
		BOUNCE_EASE_OUT,	// Fast in, bounce out.
		ELASTIC_EASE_OUT,	// Fast in, shake out.
		ZERO,
	};

	enum RepeatMode
	{
		RESTART = 0,
		REVERSE
	};

					UIAnimation(UIView* target = NULL);
					~UIAnimation();

public:
	void			Start();
	void			Stop();
	void			Reset();
	void			SetTarget(UIView* pTarget);
	UIView*			GetTarget();
	void			SetStartTime(DWORD dwStartTimeTick);
	DWORD			GetStartTime() const;
	BOOL			IsAnimating() const;
	void			SetDuration(int nDuration);
	int				GetDuration() const;
	void			SetRepeatCount(int nCount);
	int				GetRepeatCount() const;
	void			SetRepeatMode(RepeatMode mode);
	RepeatMode		GetRepeatMode() const;
	void			SetTweenType(TweenType type);
	TweenType		GetTweenType() const;
	int				GetValueBetween(int start, int target);
	double			GetValueBetween(double start, double target);
	UIRect			GetValueBetween(const UIRect start, const UIRect& target);
	COLORREF		GetValueBetween(COLORREF start, COLORREF target);
	virtual double	GetCurrentValue();
	virtual void	OnAnimationStarted();
	virtual void	OnAnimationStepped();
	virtual void	OnAnimationStopped();

protected:
	UIView* m_pTarget;
	int m_nRepeatCount;
	int m_nRepeated;
	BOOL m_bAnimating;
	BOOL m_bStarted;
	BOOL m_bCycleFlip;
	UINT m_nDuration;
	UINT m_nStartTimeTick;
	TweenType m_tweenType;
	RepeatMode m_repeatMode;
};
#endif //! __UIANIMATION_H_INCLUDED__