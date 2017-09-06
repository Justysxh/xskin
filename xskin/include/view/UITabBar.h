#ifndef __UITAB_BAR_H_INCLUDED__
#define __UITAB_BAR_H_INCLUDED__

#include "UIBoxLayout.h"

class XSKIN_API UITabBar : public UIBoxLayout
{
	DECLARE_XML_NODE(UITabBar)
public:
			UITabBar(UIView* parent = NULL);
			~UITabBar();

public:
	void	SetCheckedView(UIView* v);
	UIView*	GetCheckedView() const;
	void	SetCheckedIndex(int nIndex);
	int		GetCheckedIndex() const;

protected:
	void	OnInit() OVERRIDE;
	BOOL	OnActionPerformed(UIActionEvent* event) OVERRIDE;

protected:
	UIView* m_pChecked;
};
#endif //! __UITAB_BAR_H_INCLUDED__