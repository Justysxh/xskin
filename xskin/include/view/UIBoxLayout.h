#ifndef __UIBOX_LAYOUT_H_INCLUDED__
#define __UIBOX_LAYOUT_H_INCLUDED__

#include "UIView.h"

class XSKIN_API UIBoxLayout : public UIView
{
	DECLARE_XML_NODE(UIBoxLayout)
	DECLARE_XML_PROPERTY()

public:
					UIBoxLayout(UIView* parent = NULL);
					~UIBoxLayout();

public:
	void			SetAlignment(UIAlignment::AlignFlag horz, UIAlignment::AlignFlag vert);
	UIAlignment		GetAlignment() const;
	void			SetDirection(Direction dir);
	Direction		GetDirection() const;
	UISize			GetPreferredSize() const OVERRIDE;

protected:
	void			OnLayout() OVERRIDE;
	void			OnPaint(HDC hdc) OVERRIDE;

protected:
	UIAlignment m_alignment;
	Direction m_direction;
};

#endif //! __UIBOX_LAYOUT_H_INCLUDED__