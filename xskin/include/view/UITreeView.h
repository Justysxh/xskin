#ifndef __UITREE_VIEW_H_INCLUDED__
#define __UITREE_VIEW_H_INCLUDED__

#include "UIListView.h"
#include "UIControl.h"

class XSKIN_API UITreeItem : public UIControl
{
	DECLARE_XML_NODE(UITreeItem)
	DECLARE_XML_PROPERTY()
public:
	enum
	{	BITMAP_TEXT_SPACING = 2,
		DEFAULT_SUBITEM_INDENT_DISTANCE = 10
	};
					UITreeItem(UIView* parent = NULL);
					~UITreeItem();

public:
	void			SetIcon(const UIBitmap* pBitmap, const UIRect& rcPos);
	void			SetExpandedIcon(const UIBitmap* pBitmap, const UIRect& rcPos);
	void			SetCollapsedIcon(const UIBitmap* pBitmap, const UIRect& rcPos);
	void			SetExpanded(BOOL b);
	BOOL			IsExpaned() const;
	BOOL			IsExpandable() const;
	void			SetSubItemLoaded(BOOL b);
	BOOL			IsSubItemLoaded() const;
	int				GetDepth() const;
	int				GetIndent() const;
	void			AddSubItem(UITreeItem* v);
	int				GetSubItemCount() const;
	UITreeItem*		GetSubItem(int nIndex);
	void			SetParentItem(UITreeItem* v);
	UITreeItem*		GetParentItem() const;
	UISize			GetPreferredSize() const OVERRIDE;

protected:
	void			OnInit() OVERRIDE;
	void			OnPaint(HDC hdc) OVERRIDE;
	BOOL			OnMousePressed(UIMouseEvent* event) OVERRIDE;
	BOOL			OnMouseClicked(UIMouseEvent* event) OVERRIDE;

private:
	UIDrawable m_foldIcon;
	UIDrawable m_unfoldIcon;
	UIDrawable m_minusIcon;
	UIDrawable m_plusIcon;
	UITreeItem* m_pParentItem;
	ViewList m_subitems;
	BOOL m_bExpanded;
	BOOL m_bSubItemsLoaded;
	int m_nDepth;
};

class XSKIN_API UITabBar;
class XSKIN_API UITreeView : public UIScrollView
{
	DECLARE_XML_NODE(UITreeView)
	DECLARE_XML_PROPERTY()
public:
					UITreeView(UIView* parent = NULL);
					~UITreeView();

public:
	void			AddItem(UITreeItem* v);
	void			InsertItem(UITreeItem* v, int nIndex);
	void			RemoveItem(UITreeItem* v);
	void			RemoveItem(int nIndex);
	UITreeItem*		GetItem(int nIndex);
	void			SetCheckedItem(UITreeItem* v);
	UITreeItem*		GetCheckedItem() const;
	void			SetCheckedIndex(int nIndex);
	int				GetCheckedIndex() const;
	void			ExpandItem(UITreeItem* pItem, BOOL bExpandAll);
	void			CollapseItem(UITreeItem* pItem, BOOL bCollapseAll);
	BOOL			IsFocusFrameVisible() const OVERRIDE;
	
protected:
	void			Expand(UITreeItem* pItem, BOOL bExpandAll = FALSE);
	void			Collapse(UITreeItem* pItem, BOOL bCollapseAll = FALSE);
	void			LoadSubItems(int nItemIndex);
	void			ProcessContent() OVERRIDE;
	void			OnInit() OVERRIDE;
	BOOL			OnActionPerformed(UIActionEvent* event) OVERRIDE;

private:
	UITabBar* m_pTab;
};
#endif //! __UITREE_VIEW_H_INCLUDED__