#include "stdafx.h"
#include "../../include/view/UITreeView.h"
#include "../../include/view/UITabBar.h"
#include "../../include/view/UIScrollBar.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/event/UIMouseEvent.h"

IMPLEMENT_XML_NODE(UITreeItem)

BEGIN_XML_PROPERTY(UITreeItem)
	SET_DRAWABLE(foldIcon, m_foldIcon)
	SET_DRAWABLE(unfoldIcon, m_unfoldIcon)
	SET_DRAWABLE(minusIcon, m_minusIcon)
	SET_DRAWABLE(plusIcon, m_plusIcon)
END_XML_PROPERTY()

UITreeItem::UITreeItem(UIView* parent)
 : UIControl(parent)
{
	m_nDepth = 0;
	m_pParentItem = NULL;
	m_bExpanded = FALSE;
	m_bSubItemsLoaded = FALSE;
	SetTextAlign(UIAlignment::ALIGN_LEFT, UIAlignment::ALIGN_VCENTER);
	SetPadding(2, 2, 2, 2);
}

UITreeItem::~UITreeItem()
{

}

void UITreeItem::SetIcon(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_foldIcon.SetBitmap(pBitmap);
	m_foldIcon.SetPos(rcPos);
}

void UITreeItem::SetExpandedIcon(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_minusIcon.SetBitmap(pBitmap);
	m_minusIcon.SetPos(rcPos);
}

void UITreeItem::SetCollapsedIcon(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_plusIcon.SetBitmap(pBitmap);
	m_plusIcon.SetPos(rcPos);
}

BOOL UITreeItem::IsExpandable() const
{
	return m_subitems.size() > 0;
}

BOOL UITreeItem::IsExpaned() const
{
	return m_bExpanded;
}

void UITreeItem::SetExpanded(BOOL b)
{
	m_bExpanded = b;
}

int	UITreeItem::GetDepth() const
{
	UITreeItem* v = GetParentItem();

	return v ? v->GetDepth() + 1 : 0;
}

void UITreeItem::SetSubItemLoaded(BOOL b)
{
	m_bSubItemsLoaded = b;
}

BOOL UITreeItem::IsSubItemLoaded() const
{
	return m_bSubItemsLoaded;
}

int UITreeItem::GetIndent() const
{
	int nWidth;

	if(m_minusIcon.IsValid())
		nWidth = m_minusIcon.Width();
	else if(m_plusIcon.IsValid())
		nWidth = m_plusIcon.Width();
	else
		nWidth = DEFAULT_SUBITEM_INDENT_DISTANCE;

	return GetDepth() * nWidth;
}

int UITreeItem::GetSubItemCount() const
{
	return static_cast<int>(m_subitems.size());
}

void UITreeItem::AddSubItem(UITreeItem* v)
{
	v->SetParentItem(this);
	m_subitems.push_back(v);
}

UITreeItem* UITreeItem::GetSubItem(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetSubItemCount())
		return NULL;
	else
		return static_cast<UITreeItem*>(m_subitems[nIndex]);
}

void UITreeItem::SetParentItem(UITreeItem* v)
{
	m_pParentItem = v;
}

UITreeItem* UITreeItem::GetParentItem() const
{
	return m_pParentItem;
}

UISize UITreeItem::GetPreferredSize() const
{
	UISize size = __super::GetPreferredSize();

	if(m_bkgnd.IsValid())
	{
		size.cx = max(size.cx, m_bkgnd.Width());
		size.cy = max(size.cy, m_bkgnd.Height());
	}
	if(m_foldIcon.IsValid())
	{
		size.cx += m_foldIcon.Width();
		size.cy = max(size.cy, m_foldIcon.Height());
	}
	if(m_minusIcon.IsValid())
	{
		size.cx += m_minusIcon.Width();
		size.cy = max(size.cy, m_minusIcon.Height());
	}
	if(m_minusIcon.IsValid())
	{
		size.cx += m_minusIcon.Width();
		size.cy = max(size.cy, m_minusIcon.Height());
	}

	return size;
}

void UITreeItem::OnInit()
{
	if(GetChildViewCount() > 0)
	{
		ViewList::iterator itor = m_childViews.begin();
		while(itor!= m_childViews.end())
		{
			UITreeItem* pTreeItem = dynamic_cast<UITreeItem*>(*itor);
			if(pTreeItem)
			{
				AddSubItem(pTreeItem);
			}
			++itor;
		}
	}

	__super::OnInit();
}

void UITreeItem::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	int bkState = GetBkState();
	if(bkState == BS_NORMAL)
	{
		if(IsChecked())
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
		else
			m_bkNormal.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}
	else if(bkState == BS_HOT)
	{
		if(IsChecked())
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
		else
			m_bkHot.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}
	else if(bkState == BS_PUSHED)
	{
		if(IsChecked())
			m_bkChecked.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
		else
			m_bkHot.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}
	else if(bkState == BS_DISABLED)
	{
		m_bkDisabled.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);
	}

	rcPaint.left += m_rcPadding.left;
	rcPaint.top += m_rcPadding.top;
	rcPaint.right -= m_rcPadding.right;
	rcPaint.bottom -= m_rcPadding.bottom;

	// draw plus or minus icon.
	UIRect rcFold;
	if(IsExpaned())
	{
		rcFold.left = rcPaint.left;
		rcFold.top = rcPaint.top + (rcPaint.Height() - m_minusIcon.Height()) / 2;
		rcFold.right = rcFold.left + m_minusIcon.Width();
		rcFold.bottom = rcFold.top + m_minusIcon.Height();
		if(IsExpandable())
			m_minusIcon.Draw(hdc, rcFold, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else
	{
		rcFold.left = rcPaint.left;
		rcFold.top = rcPaint.top + (rcPaint.Height() - m_plusIcon.Height()) / 2;
		rcFold.right = rcFold.left + m_plusIcon.Width();
		rcFold.bottom = rcFold.top + m_plusIcon.Height();
		if(IsExpandable())
			m_plusIcon.Draw(hdc, rcFold, UIDrawable::FILL_NONE, m_nOpacity);
	}
	

	// draw expanded or collapsed icon.
	UIRect rcIcon;
	rcIcon.left = IsExpandable() ? rcFold.right : rcPaint.left;
	rcIcon.right = rcIcon.left + m_foldIcon.Width();
	rcIcon.top = rcPaint.top + (rcPaint.Height() - m_foldIcon.Height()) / 2;
	rcIcon.bottom = rcIcon.top + m_foldIcon.Height();

	if(IsExpaned())
		m_unfoldIcon.Draw(hdc, rcIcon, UIDrawable::FILL_NONE, m_nOpacity);
	else
		m_foldIcon.Draw(hdc, rcIcon, UIDrawable::FILL_NONE, m_nOpacity);
	

	// draw text.
	rcPaint.left = rcIcon.right + BITMAP_TEXT_SPACING;
	HFONT hOldFont = static_cast<HFONT>(::SelectObject(hdc, *m_pFont));
	COLORREF clrOld = ::SetTextColor(hdc, m_bEnabled ? m_clrText : m_clrTextDisabled);
	UIRender::DrawString(hdc, rcPaint, m_szText, m_alignment);
	::SetTextColor(hdc, clrOld);
	::SelectObject(hdc, hOldFont);
}

BOOL UITreeItem::OnMousePressed(UIMouseEvent* event)
{
	if(event->GetFlags() & UIEvent::LEFT_BUTTON_MASK)
	{
		UIActionEvent action(this, GetID(), ACTION_CLICK, 0);
		UIApplication::SendEvent(&action);
	}

	return TRUE;
}

BOOL UITreeItem::OnMouseClicked(UIMouseEvent* event)
{
	return TRUE;
}


IMPLEMENT_XML_NODE(UITreeView)

BEGIN_XML_PROPERTY(UITreeView)
END_XML_PROPERTY()

UITreeView::UITreeView(UIView* parent)
: UIScrollView(parent)
{
	m_bHScrollVisible = TRUE;
	SetScrollUnit(20);
	m_pTab = new UITabBar(this);
	m_pTab->SetDirection(VERTICAL);
	m_pTab->SetMargin(2, 2, 2, 2);
	m_pTab->SetSizePolicy(UISizePolicy::PREFERRED, UISizePolicy::PREFERRED);
}

UITreeView::~UITreeView()
{

}

void UITreeView::AddItem(UITreeItem* v)
{
	m_pTab->AddChildView(v);
}

void UITreeView::RemoveItem(UITreeItem* v)
{
	m_pTab->RemoveChildView(v);
}

void UITreeView::RemoveItem(int nIndex)
{
	m_pTab->RemoveChildView(nIndex);
}

void UITreeView::InsertItem(UITreeItem* v, int nIndex)
{
	m_pTab->AddChildView(v, nIndex);
}

UITreeItem* UITreeView::GetItem(int nIndex)
{
	return static_cast<UITreeItem*>(m_pTab->GetChildView(nIndex));
}

UITreeItem* UITreeView::GetCheckedItem() const
{
	return static_cast<UITreeItem*>(m_pTab->GetCheckedView());
}

int UITreeView::GetCheckedIndex() const
{
	return m_pTab->GetCheckedIndex();
}

void UITreeView::SetCheckedItem(UITreeItem* v)
{
	m_pTab->SetCheckedView(v);
}

void UITreeView::SetCheckedIndex(int nIndex)
{
	m_pTab->SetCheckedIndex(nIndex);
}

void UITreeView::Expand(UITreeItem* pItem, BOOL bExpandAll)
{
	if(pItem && pItem->GetParent() == m_pTab && pItem->IsExpandable())
	{
		if(!pItem->IsExpaned())
		{
			for(int x = 0; x < pItem->GetSubItemCount(); ++x)
			{
				UITreeItem* pSubItem = pItem->GetSubItem(x);
				pSubItem->SetVisible(TRUE);
				if(bExpandAll)
					Expand(pSubItem, TRUE);
			}
			pItem->SetExpanded(TRUE);
		}
	}
}

void UITreeView::Collapse(UITreeItem* pItem, BOOL bCollapseAll)
{
	if(pItem && pItem->GetParent() == m_pTab && pItem->IsExpandable())
	{
		if(pItem->IsExpaned())
		{
			for(int x = 0; x < pItem->GetSubItemCount(); ++x)
			{
				UITreeItem* pSubItem = pItem->GetSubItem(x);
				pSubItem->SetVisible(FALSE);
				if(bCollapseAll)
					Collapse(pSubItem, TRUE);
			}
			pItem->SetExpanded(FALSE);
		}
	}
}

void UITreeView::ExpandItem(UITreeItem* pItem, BOOL bExpandAll)
{
	Expand(pItem, bExpandAll);
	pItem->RequestLayout();
}

void UITreeView::CollapseItem(UITreeItem* pItem, BOOL bCollapseAll)
{
	Collapse(pItem, bCollapseAll);
	pItem->RequestLayout();
}

BOOL UITreeView::IsFocusFrameVisible() const
{
	return FALSE;
}

void UITreeView::LoadSubItems(int nItemIndex)
{
	UITreeItem* pItem = dynamic_cast<UITreeItem*>(m_pTab->GetChildView(nItemIndex));
	if(pItem && pItem->IsExpandable())
	{
		if(pItem->IsSubItemLoaded())
			return;

		for(int x = 0; x < pItem->GetSubItemCount(); ++x)
		{
			UITreeItem* pSubItem = pItem->GetSubItem(x);
			if(pSubItem->GetParent() != m_pTab)
			{
				int nIndent = pSubItem->GetIndent();
				UIRect rcPadding = pSubItem->GetPadding();
				rcPadding.left += nIndent;
				pSubItem->SetPadding(rcPadding);
				pSubItem->SetVisible(FALSE);
				InsertItem(pSubItem, ++nItemIndex);
			}
		}
		pItem->SetSubItemLoaded(TRUE);
	}
}

void UITreeView::OnInit()
{
	m_pTab->SetPadding(GetPadding());
	SetPadding(0, 0, 0, 0);

	if(GetChildViewCount() > 3)
	{
		ViewList views;
		ViewList::iterator itemBegin = m_childViews.begin() + 3;
		ViewList::iterator itemEnd = m_childViews.end();
		std::copy(itemBegin, itemEnd, std::back_inserter(views));
		m_childViews.erase(itemBegin, itemEnd);
		int nCount = views.size();
		for(int x = 0; x < nCount; ++x)
		{
			m_pTab->AddChildView(views[x]);
		}
	}

	__super::OnInit();
}

void UITreeView::ProcessContent()
{
	UIRect rcView = GetBounds();
	UISize szView = GetSize();
	UISize szHScroll = m_pHScrollBar->GetSize();
	UISize szVScroll = m_pVScrollBar->GetSize();
	UISize szAvail;

	szAvail.cx = (m_bVScrollVisible && m_pVScrollBar->IsVisible()) ? szView.cx - szVScroll.cx : szView.cx;
	szAvail.cy = (m_bHScrollVisible && m_pHScrollBar->IsVisible()) ? szView.cy - szHScroll.cy : szView.cy;

	// lay out content views.
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(!v->IsVisible() || v == m_pHScrollBar || v == m_pVScrollBar)
			continue;

		UISize size;
		UISizePolicy sp = v->GetSizePolicy();
		UIRect rcMargin = v->GetMargin();
		UISize szPref;
		if(sp.cx == UISizePolicy::PREFERRED || sp.cy == UISizePolicy::PREFERRED)
			szPref = v->GetPreferredSize();

		size.cx = max(szAvail.cx - rcMargin.left - rcMargin.right, szPref.cx);
		size.cy = szPref.cy;

		int xStart = m_ptViewportOrg.x;
		int yStart = m_ptViewportOrg.y;
		UIRect rcClip(0, 0, abs(xStart) + szAvail.cx, abs(yStart) + szAvail.cy);
		v->SetClipRect(rcClip);
		v->SetBounds(xStart + rcMargin.left, yStart + rcMargin.top, xStart + rcMargin.left + size.cx, yStart + rcMargin.top + size.cy);

		break;
	}
}

BOOL UITreeView::OnActionPerformed(UIActionEvent* event)
{
	if(event->GetSource() != m_pTab && event->GetSource() != this && event->GetCommand() == ACTION_CLICK)
	{
		// item clicked.
		RequestFocus();
		UITreeItem* v = dynamic_cast<UITreeItem*>(event->GetSource());
		if(v)
		{
			if(!v->IsSubItemLoaded())
				LoadSubItems(GetCheckedIndex());

			if(v->IsExpaned())
				CollapseItem(v, TRUE);
			else
				ExpandItem(v, FALSE);
		}
	}
	if(event->GetSource() == m_pTab && event->GetCommand() == ACTION_SELECT)
	{
		UIActionEvent action(this, GetID(), ACTION_SELECT, m_pTab->GetCheckedIndex());
		UIApplication::SendEvent(&action);
	}

	return __super::OnActionPerformed(event);
}