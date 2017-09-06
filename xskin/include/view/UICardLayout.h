#ifndef __UICARD_LAYOUT_H_INCLUDED__
#define __UICARD_LAYOUT_H_INCLUDED__

#include "UIView.h"

class XSKIN_API UICardLayout : public UIView
{
	DECLARE_XML_NODE(UICardLayout)
	DECLARE_XML_PROPERTY()
public:
			UICardLayout(UIView* parent = NULL);
			~UICardLayout();

public:
	void	First();
	void	Last();
	void	Previous();
	void	Next();
	void	Show(int nIndex);
	void	Show(const UIString& szViewID);
	UISize	GetPreferredSize() const OVERRIDE;

protected:
	void	ShowExclusive(UIView* v);
	void	OnInit() OVERRIDE;
	void	OnLayout() OVERRIDE;
	void	OnPaint(HDC hdc) OVERRIDE;

protected:
	int m_nShownIndex;
};
#endif //! __UICARD_LAYOUT_H_INCLUDED__



