#ifndef __UIBUTTON_H_INCLUDED__
#define __UIBUTTON_H_INCLUDED__

#include "UIControl.h"
#include "../core/UIString.h"

class XSKIN_API UIButton : public UIControl
{
	DECLARE_XML_NODE(UIButton)
	DECLARE_XML_PROPERTY()
public:
					UIButton(UIView* parent = NULL);
					~UIButton();

public:
	void			Activate(BOOL b) OVERRIDE;

protected:
	BOOL			OnMouseClicked(UIMouseEvent* event) OVERRIDE;
	void			OnPaint(HDC hdc) OVERRIDE;
};

#endif //! __UIBUTTON_H_INCLUDED__