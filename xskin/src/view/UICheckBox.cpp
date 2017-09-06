#include "stdafx.h"
#include "../../include/view/UICheckBox.h"
#include "../../include/core/UIApplication.h"
#include "../../include/event/UIActionEvent.h"

IMPLEMENT_XML_NODE(UICheckBox)

BEGIN_XML_PROPERTY(UICheckBox)
END_XML_PROPERTY()

UICheckBox::UICheckBox(UIView* parent)
 : UIRadioButton(parent)
{

}

UICheckBox::~UICheckBox()
{

}

void UICheckBox::SetChecked(BOOL b)
{
	if(m_bChecked == b)
		return;

	m_bChecked = b;
	Invalidate();

	UIActionEvent event(this, GetID(), ACTION_SELECT, m_bChecked);
	UIApplication::SendEvent(&event);
}

BOOL UICheckBox::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetCommand() == ACTION_CLICK)
	{
		SetChecked(m_bChecked ? FALSE : TRUE);
		return TRUE;
	}
	else
	{
		return __super::OnActionPerformed(event);
	}
}
