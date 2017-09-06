#ifndef __UICHECK_BOX_H_INCLUDED__
#define __UICHECK_BOX_H_INCLUDED__

#include "UIRadioButton.h"

class XSKIN_API UICheckBox : public UIRadioButton
{
	DECLARE_XML_NODE(UICheckBox)
	DECLARE_XML_PROPERTY()
public:
			UICheckBox(UIView* parent = NULL);
			~UICheckBox();

public:
	void	SetChecked(BOOL b) OVERRIDE;

protected:
	BOOL	OnActionPerformed(UIActionEvent* event) OVERRIDE;

};
#endif //! __UICHECK_BOX_H_INCLUDED__