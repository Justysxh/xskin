#include "stdafx.h"
#include "../../include/view/UIBoxLayout.h"

IMPLEMENT_XML_NODE(UIBoxLayout)

BEGIN_XML_PROPERTY(UIBoxLayout)
	SET_ALIGN(gravity, m_alignment)
	SET_DIRECTION(direction, m_direction)
END_XML_PROPERTY()

UIBoxLayout::UIBoxLayout(UIView* parent)
 : UIView(parent)
{
	m_direction = HORIZONTAL;
}

UIBoxLayout::~UIBoxLayout()
{

}

void UIBoxLayout::SetAlignment(UIAlignment::AlignFlag horz, UIAlignment::AlignFlag vert)
{
	m_alignment.cx = horz;
	m_alignment.cy = vert;
}

UIAlignment UIBoxLayout::GetAlignment() const
{
	return m_alignment;
}

void UIBoxLayout::SetDirection(Direction dir)
{
	m_direction = dir;
}

Direction UIBoxLayout::GetDirection() const
{
	return m_direction;
}

void UIBoxLayout::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH);
}

void UIBoxLayout::OnLayout()
{
	UISize szCenter;
	UISize szParent = GetSize();
	UISize szFree = szParent;
	szFree.cx -= m_rcPadding.left + m_rcPadding.right;
	szFree.cy -= m_rcPadding.top + m_rcPadding.bottom;
	int xStart = -1;
	int yStart = -1;
	for(int i = 0, count = GetChildViewCount(); i < count; ++i)
	{
		UIView* v = NULL;
		if((m_direction == HORIZONTAL && m_alignment.cx == UIAlignment::ALIGN_RIGHT)
			|| (m_direction == VERTICAL && m_alignment.cy == UIAlignment::ALIGN_BOTTOM))
			v = GetChildView(count - i - 1);
		else
			v = GetChildView(i);

		if(!v->IsVisible())
			continue;

		UISize size;
		UIRect bound = v->GetBounds();
		UIRect margin = v->GetMargin();
		UISizePolicy sp = v->GetSizePolicy();
		
		UISize szPref;
		if(sp.cx == UISizePolicy::PREFERRED || sp.cy == UISizePolicy::PREFERRED)
			szPref = v->GetPreferredSize();

		if(sp.cx == UISizePolicy::FIXED)
			size.cx = v->GetSize().cx;
		else if(sp.cx == UISizePolicy::PREFERRED)
			size.cx = szPref.cx;
		else if(sp.cx == UISizePolicy::EXPANDING)
			size.cx = szFree.cx - margin.left - margin.right;

		if(sp.cy == UISizePolicy::FIXED)
			size.cy = v->GetSize().cy;
		else if(sp.cy == UISizePolicy::PREFERRED)
			size.cy = szPref.cy;
		else if(sp.cy == UISizePolicy::EXPANDING)
			size.cy = szFree.cy - margin.top - margin.bottom;

		if(m_direction == HORIZONTAL)
		{
			if(m_alignment.cx == UIAlignment::ALIGN_LEFT)
			{
				if(xStart == -1)
					xStart = m_rcPadding.left;
				bound.left = xStart + margin.left;
				bound.right = bound.left + size.cx;
				xStart += margin.left + size.cx + margin.right;
			}
			else if(m_alignment.cx == UIAlignment::ALIGN_RIGHT)
			{
				if(xStart == -1)
					xStart = szParent.cx - m_rcPadding.right;
				bound.right = xStart - margin.right;
				bound.left = bound.right - size.cx;
				xStart -= (margin.right + size.cx + margin.left);
			}
			else if(m_alignment.cx == UIAlignment::ALIGN_HCENTER)
			{
				szCenter.cx += margin.left + size.cx + margin.right;
				bound.left = 0;
				bound.right = size.cx;
			}

			if(m_alignment.cy == UIAlignment::ALIGN_TOP)
			{
				bound.top = m_rcPadding.top + margin.top;
				bound.bottom = bound.top + size.cy;
			}
			else if(m_alignment.cy == UIAlignment::ALIGN_BOTTOM)
			{
				bound.bottom = szParent.cy - m_rcPadding.bottom - margin.bottom;
				bound.top = bound.bottom - size.cy;
			}
			else if(m_alignment.cy == UIAlignment::ALIGN_VCENTER)
			{
				bound.top = (szParent.cy - size.cy) / 2;
				bound.bottom = bound.top + size.cy;
			}

			szFree.cx -= (margin.left + bound.Width() + margin.right);
		}
		else
		{
			if(m_alignment.cy == UIAlignment::ALIGN_TOP)
			{
				if(yStart == -1)
					yStart = m_rcPadding.top;
				bound.top = yStart + margin.top;
				bound.bottom = bound.top + size.cy;
				yStart += margin.top + size.cy + margin.bottom;
			}
			else if(m_alignment.cy == UIAlignment::ALIGN_BOTTOM)
			{
				if(yStart == -1)
					yStart = szParent.cy - m_rcPadding.bottom;
				bound.bottom = yStart - margin.bottom;
				bound.top = bound.bottom - size.cy;
				yStart -= (margin.bottom + size.cy + margin.top);
			}
			else if(m_alignment.cy == UIAlignment::ALIGN_VCENTER)
			{
				szCenter.cy += margin.top + size.cy + margin.right;
				bound.top = 0;
				bound.bottom = size.cy;
			}

			if(m_alignment.cx == UIAlignment::ALIGN_LEFT)
			{
				bound.left = m_rcPadding.left + margin.left;
				bound.right = bound.left + size.cx;
			}
			else if(m_alignment.cx == UIAlignment::ALIGN_RIGHT)
			{
				bound.right = szParent.cx - m_rcPadding.right - margin.right;
				bound.left = bound.right - size.cx;
			}
			if(m_alignment.cx == UIAlignment::ALIGN_HCENTER)
			{
				bound.left = (szParent.cx - size.cx) / 2;
				bound.right = bound.left + size.cx;
			}

			szFree.cy -= (margin.top + bound.Height() + margin.bottom);
		}

		if(szFree.cx < 0) szFree.cx = 0;
		if(szFree.cy < 0) szFree.cy = 0;

		v->SetBounds(bound);
	}

	// process h-center and v-center alignment.
	if(m_alignment.cx == UIAlignment::ALIGN_HCENTER || m_alignment.cy == UIAlignment::ALIGN_VCENTER)
	{
		int xStart = (szParent.cx - szCenter.cx) / 2;
		int yStart = (szParent.cy - szCenter.cy) / 2;
		for(int j = 0; j < GetChildViewCount(); ++j)
		{
			UIView* v = GetChildView(j);
			if(!v->IsVisible())
				continue;

			UIRect bound;
			UIRect rcBound = v->GetBounds();
			UISize size = v->GetSize();
			UIRect margin = v->GetMargin();
			if(szCenter.cx > 0)
			{
				bound.left = xStart + margin.left;
				bound.right = bound.left + size.cx;
				xStart += margin.left + size.cx + margin.right;
				v->SetBounds(bound.left, rcBound.top, bound.right, rcBound.bottom);
			}
			if(szCenter.cy > 0)
			{
				bound.top = yStart + margin.top;
				bound.bottom = bound.top + size.cy;
				yStart += margin.top + size.cy + margin.bottom;
				v->SetBounds(rcBound.left, bound.top, rcBound.right, bound.bottom);
			}
		}
	}

	__super::OnLayout();
}

UISize UIBoxLayout::GetPreferredSize() const
{
	UIRect rcBound;
	UIRect bound;
	int nPos = 0;
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(!v->IsVisible())
			continue;

		UISize size;
		UIRect margin = v->GetMargin();
		UISizePolicy sp = v->GetSizePolicy();
		UISize szPref = v->GetPreferredSize();

		if(sp.cx == UISizePolicy::FIXED)
			size.cx = v->GetSize().cx;
		else if(sp.cx == UISizePolicy::PREFERRED)
			size.cx = szPref.cx;
		else if(sp.cx == UISizePolicy::EXPANDING)
			size.cx = szPref.cx;

		if(sp.cy == UISizePolicy::FIXED)
			size.cy = v->GetSize().cy;
		else if(sp.cy == UISizePolicy::PREFERRED)
			size.cy = szPref.cy;
		else if(sp.cy == UISizePolicy::EXPANDING)
			size.cy = szPref.cy;

		if(m_direction == HORIZONTAL)
		{
			bound.SetRect(nPos, 0, nPos + margin.left + size.cx + margin.right, margin.top + size.cy + margin.bottom);
			rcBound.UnionRect(rcBound, bound);
			nPos += margin.left + size.cx + margin.right;
		}
		else
		{
			bound.SetRect(0, nPos, margin.left + size.cx + margin.right, nPos + margin.top + size.cy + margin.bottom);
			rcBound.UnionRect(rcBound, bound);
			nPos += margin.top + size.cy + margin.bottom;
		}
	}

	rcBound.left -= m_rcPadding.left;
	rcBound.top -= m_rcPadding.top;
	rcBound.right += m_rcPadding.right;
	rcBound.bottom += m_rcPadding.bottom;

	return rcBound;
}