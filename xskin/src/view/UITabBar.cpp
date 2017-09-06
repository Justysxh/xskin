#include "stdafx.h"
#include "../../include/view/UITabBar.h"
#include "../../include/event/UIMouseEvent.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/core/UIApplication.h"

IMPLEMENT_XML_NODE(UITabBar)

UITabBar::UITabBar(UIView* parent)
 : UIBoxLayout(parent)
{
	m_pChecked = NULL;
}

UITabBar::~UITabBar()
{

}

void UITabBar::SetCheckedIndex(int nIndex)
{
	UIView* v = GetChildView(nIndex);
	if(v)
		SetCheckedView(v);
}

void UITabBar::SetCheckedView(UIView* v)
{
	if(m_pChecked == v)
		return;

	if(v && v->IsEnabled() && !v->IsChecked())
	{
		v->SetChecked(TRUE);
		m_pChecked = v;

		int nIndex = 0;
		for(int x = 0; x < GetChildViewCount(); ++x)
		{
			UIView* pChild = GetChildView(x);
			if(pChild == v)
			{
				nIndex = x;
				continue;
			}
			else if(pChild->IsChecked())
			{
				pChild->SetChecked(FALSE);
			}
		}

		UIActionEvent event(this, GetID(), ACTION_SELECT, nIndex);
		UIApplication::SendEvent(&event);
	}
}

UIView* UITabBar::GetCheckedView() const
{
	return m_pChecked;
}

int UITabBar::GetCheckedIndex() const
{
	int nCheckedIndex = 0;
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		if(GetChildView(x) == m_pChecked)
		{
			nCheckedIndex = x;
			break;
		}
	}

	return nCheckedIndex;
}

void UITabBar::OnInit()
{
	if(!m_pChecked)
	{
		if(GetChildViewCount() > 0)
			SetCheckedView(GetChildView(0));
	}
	__super::OnInit();
}

BOOL UITabBar::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetSource() != this && event->GetCommand() == ACTION_CLICK)
	{
		UIView* v = static_cast<UIView*>(event->GetSource());
		SetCheckedView(v);
		return FALSE;
	}
	else
	{
		return __super::OnActionPerformed(event);
	}
}