#ifndef __UIPROGRESS_BAR_H_INCLUDED__
#define __UIPROGRESS_BAR_H_INCLUDED__

#include "UIControl.h"

class XSKIN_API UIProgressBar : public UIControl
{
	DECLARE_XML_NODE(UIProgressBar)
	DECLARE_XML_PROPERTY()
public:
					UIProgressBar(UIView* parent = NULL);
					~UIProgressBar();

public:
	void			SetDirection(Direction dir);
	Direction		GetDirection() const;
	void			SetTextVisible(BOOL b);
	void			SetValue(int nValue);
	int				GetValue() const;
	void			SetRange(int nMin, int nMax);
	UISize			GetRange() const;
	UISize			GetPreferredSize() const OVERRIDE;

protected:
	int				Clamp(int x, int a, int b);
	void			OnPaint(HDC hdc) OVERRIDE;

protected:
	Direction m_direction;
	int m_nValue;
	BOOL m_bTextVisible;
	UISize m_szRange;
	UIDrawable m_bkFragment;
};

#endif //! __UIPROGRESS_BAR_H_INCLUDED__