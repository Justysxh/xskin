#include "stdafx.h"
#include "../../include/view/UIListView.h"
#include "../../include/view/UITabBar.h"
#include "../../include/view/UIScrollBar.h"
#include "../../include/view/UIListItem.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/core/UIApplication.h"
#include "../../include/model/UIModel.h"

IMPLEMENT_XML_NODE(UIListView)

BEGIN_XML_PROPERTY(UIListView)
END_XML_PROPERTY()

UIListView::UIListView(UIView* parent)
 : UIScrollView(parent)
{
	SetScrollUnit(DEFAULT_ITEM_HEIGHT);
	SetPadding(2, 2, 2, 2);
	m_bReload = TRUE;
	m_pModel = NULL;
	m_bHScrollVisible = FALSE;
	m_pCheckedItem = NULL;
}

UIListView::~UIListView()
{
	if(m_pModel)
		delete m_pModel;
}

void UIListView::SetModel(UIModel* model)
{
	if(m_pModel)
	{
		DeleteItems();
		m_pModel = NULL;
	}

	m_pModel = model;
	NotifyDataChanged();
}

UIModel* UIListView::GetModel() const
{
	return m_pModel;
}

void UIListView::NotifyDataChanged()
{
	m_bReload = TRUE;
	InvalidateLayout();
	Invalidate();
}

UIView* UIListView::GetItem(int nIndex)
{
	return GetChildView(nIndex);
}

UIView* UIListView::GetCheckedItem() const
{
	return m_pCheckedItem;
}

int UIListView::GetCheckedIndex() const
{
	int nCheckedIndex = 0;
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		if(GetChildView(x) == m_pCheckedItem)
		{
			nCheckedIndex = x;
			break;
		}
	}

	return nCheckedIndex;
}

void UIListView::SetCheckedItem(UIView* v)
{
	if(v && v->IsEnabled())
	{
		v->SetChecked(TRUE);
		m_pCheckedItem = v;

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

void UIListView::SetCheckedIndex(int nIndex)
{
	UIView* v = GetChildView(nIndex);
	if(v)
		SetCheckedItem(v);
}

BOOL UIListView::IsFocusFrameVisible() const
{
	return FALSE;
}

UISize UIListView::GetPreferredSize() const
{
	UIRect rcBound;
	UIRect bound;
	int nPos = 0;
	for (int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if (!v->IsVisible() || v == m_pVScrollBar || v == m_pHScrollBar)
			continue;

		UISize size;
		UIRect margin = v->GetMargin();
		UISizePolicy sp = v->GetSizePolicy();
		UISize szPref = v->GetPreferredSize();

		if (sp.cx == UISizePolicy::FIXED)
			size.cx = v->GetSize().cx;
		else if (sp.cx == UISizePolicy::PREFERRED)
			size.cx = szPref.cx;
		else if (sp.cx == UISizePolicy::EXPANDING)
			size.cx = szPref.cx;

		if (sp.cy == UISizePolicy::FIXED)
			size.cy = v->GetSize().cy;
		else if (sp.cy == UISizePolicy::PREFERRED)
			size.cy = szPref.cy;
		else if (sp.cy == UISizePolicy::EXPANDING)
			size.cy = szPref.cy;

		bound.SetRect(0, nPos, margin.left + size.cx + margin.right, nPos + margin.top + size.cy + margin.bottom);
		rcBound.UnionRect(rcBound, bound);
		nPos += margin.top + size.cy + margin.bottom;
	}

	rcBound.left -= m_rcPadding.left;
	rcBound.top -= m_rcPadding.top;
	rcBound.right += m_rcPadding.right;
	rcBound.bottom += m_rcPadding.bottom;

	return rcBound;
}

void UIListView::DeleteItems()
{
	RemoveAllChildView();
	std::vector<UIView*>::iterator it;
	while((it = m_items.begin()) != m_items.end())
	{
		delete *it;
		m_items.erase(it);
	}
}

void UIListView::Scroll(int x, int y)
{
	UISize szView = GetSize();
	UIRect rcView = GetBounds();
	UISize szHScroll = m_pHScrollBar->GetSize();
	UISize szVScroll = m_pVScrollBar->GetSize();
	UISize szAvail;
	szAvail.cx = (m_bVScrollVisible && m_pVScrollBar->IsVisible()) ? szView.cx - szVScroll.cx : szView.cx;
	szAvail.cx -= m_rcPadding.left + m_rcPadding.right;
	szAvail.cy = (m_bHScrollVisible && m_pHScrollBar->IsVisible()) ? szView.cy - szHScroll.cy : szView.cy;
	szAvail.cy -= m_rcPadding.top + m_rcPadding.bottom;

	int nOffset = -1;

	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(!v->IsVisible() || v == m_pHScrollBar || v == m_pVScrollBar)
			continue;

		UISize size = v->GetSize();
		UIRect margin = v->GetMargin();

		if(nOffset == -1)
			nOffset = m_rcPadding.top;

		UIRect bound;
		bound.left = margin.left + m_rcPadding.left;
		bound.top = y + nOffset + margin.top;
		bound.right = bound.left + size.cx;
		bound.bottom = bound.top + size.cy;
		nOffset += margin.top + size.cy + margin.bottom;

		v->SetClipRect(0, 0, szAvail.cx, abs(m_ptViewportOrg.y) + szAvail.cy);
		v->SetBounds(bound);
	}
}

void UIListView::OnLayout()
{
	if(m_pModel && !m_pModel->IsEmpty() && m_bReload)
	{
		RemoveAllChildView();
		std::vector<UIView*> tmp;
		for(int x = 0; x < m_pModel->GetCount(); ++x)
		{
			UIView* cv = m_items.size() > x ? m_items[x] : NULL;
			UIView* v = m_pModel->GetView(cv, x);
			if(cv != v)
				tmp.push_back(v);
			AddChildView(v);
		}

		AddChildView(m_pHScrollBar);
		AddChildView(m_pVScrollBar);
		m_items.insert(m_items.end(), tmp.begin(), tmp.end());
		m_bReload = FALSE;
	}

	ProcessScrollbar();
	ProcessContent();

	__super::OnLayout();
}

void UIListView::ProcessContent()
{
	UISize szView = GetSize();
	UISize szHScroll = m_pHScrollBar->GetSize();
	UISize szVScroll = m_pVScrollBar->GetSize();
	UISize szAvail;
	szAvail.cx = (m_bVScrollVisible && m_pVScrollBar->IsVisible()) ? szView.cx - szVScroll.cx : szView.cx;
	szAvail.cx -= m_rcPadding.left + m_rcPadding.right;
	szAvail.cy = (m_bHScrollVisible && m_pHScrollBar->IsVisible()) ? szView.cy - szHScroll.cy : szView.cy;
	szAvail.cy -= m_rcPadding.top + m_rcPadding.bottom;

	int nOffset = -1;

	for (int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if (!v->IsVisible() || v == m_pHScrollBar || v == m_pVScrollBar)
			continue;

		UISize size;
		UISizePolicy sp = v->GetSizePolicy();
		UIRect margin = v->GetMargin();

		size.cx = szAvail.cx - margin.left - margin.right;

		if (sp.cy == UISizePolicy::FIXED)
			size.cy = v->GetSize().cy;
		else
			size.cy = v->GetPreferredSize().cy;

		if (nOffset == -1)
			nOffset = m_rcPadding.top;

		UIRect bound;
		bound.left = margin.left + m_rcPadding.left;
		bound.top = m_ptViewportOrg.y + nOffset + margin.top;
		bound.right = bound.left + size.cx;
		bound.bottom = bound.top + size.cy;
		nOffset += margin.top + size.cy + margin.bottom;
		
		v->SetClipRect(0, 0, szAvail.cx, abs(m_ptViewportOrg.y) + szAvail.cy);
		v->SetBounds(bound);
	}
}

BOOL UIListView::OnActionPerformed(UIActionEvent* event)
{
	UIView* v = static_cast<UIView*>(event->GetSource());
	if(event->GetCommand() == ACTION_CLICK)
	{
		UIListItem* item = dynamic_cast<UIListItem*>(v);
		if(item)
		{
			RequestFocus();
			SetCheckedItem(v);
			return TRUE;
		}
	}
	else if(event->GetCommand() == ACTION_SELECT && event->GetSource() != this)
	{
		UIActionEvent action(this, GetID(), ACTION_SELECT, event->GetFlags());
		UIApplication::SendEvent(&action);
		return TRUE;
	}

	return __super::OnActionPerformed(event);;
}