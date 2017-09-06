#ifndef __UISLIDER_H_INCLUDED__
#define __UISLIDER_H_INCLUDED__

#include "UIProgressBar.h"

class XSKIN_API UISlider : public UIProgressBar
{
protected:
	class XSKIN_API UISliderButton : public UIControl
	{
	public:
				UISliderButton(UIView* parent = NULL);
				~UISliderButton();

	protected:
		void	OnPaint(HDC hdc) OVERRIDE;
		BOOL	OnMousePressed(UIMouseEvent* event) OVERRIDE;
		BOOL	OnMouseDragged(UIMouseEvent* event) OVERRIDE;
	};

	DECLARE_XML_NODE(UISlider)
	DECLARE_XML_PROPERTY()
public:
	enum { DEFAULT_STEP_UNIT = 1 };

				UISlider(UIView* parent = NULL);
				~UISlider();

public:
	void		SetStepUnit(int nUnit);
	int			GetStepUnit() const;
	void		StepForward();
	void		StepBackward();
	void		SetEnabled(BOOL b) OVERRIDE;
	UISize		GetPreferredSize() const OVERRIDE;

protected:
	void		UpdateTrackPos();
	void		SetTrackPos(const UIRect& rcTrack);
	void		OnLayout() OVERRIDE;
	void		OnPaint(HDC hdc) OVERRIDE;
	void		OnInit() OVERRIDE;
	BOOL		OnMousePressed(UIMouseEvent* event) OVERRIDE;
	BOOL		OnMouseDragged(UIMouseEvent* event) OVERRIDE;
	BOOL		OnKeyPressed(UIKeyEvent* event) OVERRIDE;
	BOOL		OnActionPerformed(UIActionEvent* event) OVERRIDE;

protected:
	int m_nStepUnit;
	UIPoint m_ptLast;
	UISliderButton*	m_pHandle;
};
#endif //! __UISLIDER_H_INCLUDED__