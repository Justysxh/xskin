#ifndef __UIGRID_LAYOUT_H_INCLUDED__
#define __UIGRID_LAYOUT_H_INCLUDED__

#include "UIView.h"

class XSKIN_API UIGridLayout : public UIView
{
	DECLARE_XML_NODE(UIGridLayout)
	DECLARE_XML_PROPERTY()
public:
	enum 
	{
		DEFAULT_HORIZONTAL_SPACING = 2,
		DEFAULT_VERTICAL_SPACING = 2
	};

				UIGridLayout(UIView* parent = NULL);
				UIGridLayout(int nRowCount, int nColumnCount, UIView* parent = NULL);
				~UIGridLayout();

public:
	void		AddView(UIView* v, int nRow, int nColumn,int nRowSpan = 1, int nColumnSpan = 1, const UIAlignment& alignment = UIAlignment());
	void		AddView(UIView* v, int nRow, int nColumn, const UIAlignment& align = UIAlignment());
	UIView*		GetView(int nRow, int nColumn) const;
	void		SetHorizontalSpacing(int nSpacing);
	void		SetVerticalSpacing(int nSpacing);
	UISize		GetSpacing() const;
	void		SetRowCount(int nRowCount);
	int			SetColumnCount(int nColumnCount);
	int			GetRowCount() const;
	int			GetColumnCount() const;
	UISize		GetPreferredSize() const OVERRIDE;

protected:
	UISize		CalcRowColumn() const;
	void		OnLayout() OVERRIDE;
	void		OnPaint(HDC hdc) OVERRIDE;

protected:
	int m_nRowCount;
	int m_nColumnCount;
	int m_nHorizontalSpacing;
	int m_nVerticalSpacing;
};
#endif // __UIGRID_LAYOUT_H_INCLUDED__