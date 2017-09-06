#include "stdafx.h"
#include "../../include/view/UIGridLayout.h"

IMPLEMENT_XML_NODE(UIGridLayout)

BEGIN_XML_PROPERTY(UIGridLayout)
	SET_INT(rowCount, m_nRowCount)
	SET_INT(columnCount, m_nColumnCount)
	SET_INT(vgap, m_nVerticalSpacing)
	SET_INT(hgap, m_nHorizontalSpacing)
END_XML_PROPERTY()

UIGridLayout::UIGridLayout(UIView* parent)
 : UIView(parent)
{
	m_nRowCount = 0;
	m_nColumnCount = 0;
	m_nHorizontalSpacing = DEFAULT_HORIZONTAL_SPACING;
	m_nVerticalSpacing = DEFAULT_VERTICAL_SPACING;
}

UIGridLayout::UIGridLayout(int nRowCount, int nColumnCount, UIView* parent)
	: UIView(parent)
{
	m_nRowCount = nRowCount;
	m_nColumnCount = nColumnCount;
	m_nHorizontalSpacing = DEFAULT_HORIZONTAL_SPACING;
	m_nVerticalSpacing = DEFAULT_VERTICAL_SPACING;
}


UIGridLayout::~UIGridLayout()
{

}

void UIGridLayout::AddView(UIView* v, int nRow, int nColumn, int nRowSpan, int nColumnSpan, const UIAlignment& alignment)
{
	m_params.nRow = nRow;
	m_params.nColumn = nColumn;
	m_params.nRowSpan = nRowSpan;
	m_params.nColumnSpan = nColumnSpan;
	m_params.alignment = alignment;
	AddChildView(v);
}

void UIGridLayout::AddView(UIView* v, int nRow, int nColumn, const UIAlignment& alignment)
{
	AddView(v, nRow, nColumn, 1, 1, alignment);
}

UIView* UIGridLayout::GetView(int nRow, int nColumn) const
{
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		ExtendedLayoutParams props = v->GetExtendedLayoutParams();
		if(props.nRow == nRow && props.nColumn == nColumn)
		{
			return v;
		}
	}

	return NULL;
}

void UIGridLayout::SetHorizontalSpacing(int nSpacing)
{
	m_nHorizontalSpacing = nSpacing;
}

void UIGridLayout::SetVerticalSpacing(int nSpacing)
{
	m_nVerticalSpacing = nSpacing;
}

UISize UIGridLayout::GetSpacing() const
{
	return UISize(m_nHorizontalSpacing, m_nVerticalSpacing);
}

int UIGridLayout::GetRowCount() const
{
	return m_nRowCount;
}

int UIGridLayout::GetColumnCount() const
{
	return m_nColumnCount;
}

UISize UIGridLayout::GetPreferredSize() const
{
	UIRect bound;
	UIRect temp;
	UISize szView;
	UISize szRowColumn = CalcRowColumn();
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		UIRect rcMagin = v->GetMargin();
		UISize szFixed = v->GetSize();
		UISizePolicy sp = v->GetSizePolicy();
		
		UISize szPref;
		if(sp.cx == UISizePolicy::PREFERRED || sp.cy == UISizePolicy::PREFERRED)
			szPref = v->GetPreferredSize();

		if(sp.cx == UISizePolicy::EXPANDING)
			szView.cx = szPref.cx;
		else if(sp.cx == UISizePolicy::PREFERRED)
			szView.cx = szPref.cx;
		else
			szView.cx = szFixed.cx;

		if(sp.cy == UISizePolicy::EXPANDING)
			szView.cy = szPref.cy;
		else if(sp.cy == UISizePolicy::PREFERRED)
			szView.cy = szPref.cy;
		else
			szView.cy = szFixed.cy;

		temp.SetRect(0, 0, rcMagin.left + szView.cx + rcMagin.right, rcMagin.top + szView.cy + rcMagin.bottom);
		bound.UnionRect(bound, temp);
	}

	int nCellWidth = bound.Width();
	int nCellHeight = bound.Height();

	int nWidth = szRowColumn.cy * (nCellWidth + m_nHorizontalSpacing) - m_nHorizontalSpacing + m_rcPadding.left + m_rcPadding.right;
	int nHeight = szRowColumn.cx * (nCellHeight + m_nVerticalSpacing) - m_nVerticalSpacing + m_rcPadding.top + m_rcPadding.bottom;

	return UISize(nWidth, nHeight);
}


UISize UIGridLayout::CalcRowColumn() const
{
	int nCount = GetChildViewCount();
	int nRowCount = m_nRowCount;
	int nColumnCount = m_nColumnCount;
	if(nCount)
	{
		if(nRowCount == 0 && nColumnCount == 0)
		{
			nRowCount = 1;
			nColumnCount = static_cast<int>(ceil(static_cast<double>(nCount) / nRowCount));
		}
		else if(nRowCount != 0 && nColumnCount == 0)
		{
			nColumnCount = static_cast<int>(ceil(static_cast<double>(nCount) / nRowCount));
		}
		else if(nRowCount == 0 && nColumnCount != 0)
		{
			nRowCount = static_cast<int>(ceil(static_cast<double>(nCount) / nColumnCount));
		}
		else if(nRowCount * nColumnCount < nCount)
		{
			nRowCount = static_cast<int>(ceil(static_cast<double>(nCount) / nColumnCount));
		}
	}

	return UISize(nRowCount, nColumnCount);
}

void UIGridLayout::OnLayout()
{
	int nCount = GetChildViewCount();
	if(!nCount)
		return;

	UISize szRowCol = CalcRowColumn();
	m_nRowCount = szRowCol.cx;
	m_nColumnCount = szRowCol.cy;

	UIRect rcCell;
	UIRect rcView;
	UISize szView;
	UISize szParent = GetSize();
	szParent.cx -= m_rcPadding.left + m_rcPadding.right;
	szParent.cy -= m_rcPadding.top + m_rcPadding.bottom;
	int nCellWidth = static_cast<int>((szParent.cx - (m_nColumnCount - 1) * m_nHorizontalSpacing) / m_nColumnCount);
	int nCellHeight = static_cast<int>((szParent.cy - (m_nRowCount - 1) * m_nVerticalSpacing) / m_nRowCount);
	for(int x = 0; x < nCount; ++x)
	{
		UIView* v = GetChildView(x);

		// calculate cell bounding.
		ExtendedLayoutParams params = v->GetExtendedLayoutParams();
		if(params.nRow == -1 && params.nColumn == -1)
		{
			params.nRow = x / m_nColumnCount;
			params.nColumn = x % m_nColumnCount;
		}

		params.nRowSpan = params.nRowSpan > m_nRowCount ? -1 : params.nRowSpan;
		params.nColumnSpan = params.nColumnSpan > m_nColumnCount ? -1 : params.nColumnSpan;
		params.nRowSpan = (params.nRowSpan == -1) ? m_nRowCount - params.nRow : params.nRowSpan;
		params.nColumnSpan = (params.nColumnSpan == -1) ? m_nColumnCount - params.nColumn : params.nColumnSpan;
		rcCell.left = params.nColumn * (nCellWidth + m_nHorizontalSpacing) + m_rcPadding.left;
		rcCell.top = params.nRow * (nCellHeight + m_nVerticalSpacing) + m_rcPadding.top;
		rcCell.right = rcCell.left + nCellWidth + (params.nColumnSpan - 1) * (m_nHorizontalSpacing + nCellWidth);
		rcCell.bottom = rcCell.top + nCellHeight + (params.nRowSpan - 1) * (m_nVerticalSpacing + nCellHeight);

		// calculate view size.
		UISizePolicy sp = v->GetSizePolicy();
		UIRect rcMargin = v->GetMargin();
		UISize szFixed = v->GetSize();
		UISize szPref = v->GetPreferredSize();
		if(sp.cx == UISizePolicy::EXPANDING)
			szView.cx = rcCell.Width() - rcMargin.left - rcMargin.right;
		else if(sp.cx == UISizePolicy::PREFERRED)
			szView.cx = szPref.cx;
		else
			szView.cx = szFixed.cx;

		if(sp.cy == UISizePolicy::EXPANDING)
			szView.cy = rcCell.Height() - rcMargin.top - rcMargin.bottom;
		else if(sp.cy == UISizePolicy::PREFERRED)
			szView.cy = szPref.cy;
		else
			szView.cy = szFixed.cy;

		// calculate view position
		if(params.alignment.cx == UIAlignment::ALIGN_LEFT)
		{
			rcView.left = rcCell.left + rcMargin.left;
			rcView.right = rcView.left + szView.cx;
		}
		else if(params.alignment.cx == UIAlignment::ALIGN_RIGHT)
		{
			rcView.right = rcCell.right - rcMargin.right;
			rcView.left = rcView.right - szView.cx;
		}
		else
		{
			rcView.left = rcCell.left + (rcCell.Width() - szView.cx) / 2;
			rcView.right = rcView.left + szView.cx;
		}

		if(params.alignment.cy == UIAlignment::ALIGN_TOP)
		{
			rcView.top = rcCell.top + rcMargin.top;
			rcView.bottom = rcView.top + szView.cy;
		}
		else if(params.alignment.cy == UIAlignment::ALIGN_BOTTOM)
		{
			rcView.bottom = rcCell.bottom - rcMargin.bottom;
			rcView.top = rcView.bottom - szView.cy;
		}
		else
		{
			rcView.top = rcCell.top + (rcCell.Height() - szView.cy) / 2;
			rcView.bottom = rcView.top + szView.cy;
		}

		v->SetBounds(rcView);
	}

	__super::OnLayout();
}

void UIGridLayout::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH);
}