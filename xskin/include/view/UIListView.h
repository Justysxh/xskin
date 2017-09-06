#ifndef __UILIST_VIEW_H_INCLUDED__
#define __UILIST_VIEW_H_INCLUDED__

#include "UIScrollView.h"

class UIModel;
class XSKIN_API UIListView : public UIScrollView
{
	DECLARE_XML_NODE(UIListView)
	DECLARE_XML_PROPERTY()
public:
	enum { DEFAULT_ITEM_HEIGHT = 20 };

				UIListView(UIView* parent = NULL);
				~UIListView();

public:
	void		SetModel(UIModel* model);
	UIModel*	GetModel() const;
	void		NotifyDataChanged();
	UIView*		GetItem(int nIndex);
	void		SetCheckedItem(UIView* v);
	void		SetCheckedIndex(int nIndex);
	UIView*		GetCheckedItem() const;
	int			GetCheckedIndex() const;
	UISize		GetPreferredSize() const OVERRIDE;
	BOOL		IsFocusFrameVisible() const OVERRIDE;

protected:
	void		DeleteItems();
	void		Scroll(int x, int y) OVERRIDE;
	void		OnLayout() OVERRIDE;
	void		ProcessContent() OVERRIDE;
	BOOL		OnActionPerformed(UIActionEvent* event) OVERRIDE;

protected:
	BOOL m_bReload;
	std::vector<UIView*> m_items;
	UIModel* m_pModel;
	UIView* m_pCheckedItem;
};
#endif //! __UILIST_VIEW_H_INCLUDED__