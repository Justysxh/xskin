#include "stdafx.h"
#include "../../include/view/UICardLayout.h"

IMPLEMENT_XML_NODE(UICardLayout)

BEGIN_XML_PROPERTY(UICardLayout)
END_XML_PROPERTY()

UICardLayout::UICardLayout(UIView* parent)
 : UIView(parent)
{
	m_nShownIndex = -1;
}

UICardLayout::~UICardLayout()
{

}

void UICardLayout::First()
{
	Show(0);
}

void UICardLayout::Last()
{
	Show(GetChildViewCount() - 1);
}

void UICardLayout::Previous()
{
	if(m_nShownIndex > 0)
		Show(m_nShownIndex - 1);
}

void UICardLayout::Next()
{
	if(m_nShownIndex < GetChildViewCount() - 1)
		Show(m_nShownIndex + 1);
}

void UICardLayout::Show(int nIndex)
{
	if(nIndex >= 0 && nIndex < GetChildViewCount())
	{
		UIView* v = GetChildView(nIndex);
		if(v)
		{
			m_nShownIndex = nIndex;
			ShowExclusive(v);
		}
	}
}

void UICardLayout::Show(const UIString& szViewID)
{
	bool b = m_childViews.empty();
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		if(v->GetID() == szViewID)
		{
			Show(x);
			break;
		}
	}

	int x = 1;
}

void UICardLayout::ShowExclusive(UIView* v)
{
	if(v)
	{
		v->SetVisible(TRUE);
		for(int x = 0; x < GetChildViewCount(); ++x)
		{
			UIView* pChild = GetChildView(x);
			if(pChild != v)
				pChild->SetVisible(FALSE);
		}
	}
}

UISize UICardLayout::GetPreferredSize() const
{
	UISize szView = GetSize();
	UIRect bound;
	UIRect rcTemp;
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UISize size;
		UIView* v = GetChildView(x);
		UIRect rcMargin = v->GetMargin();
		UISize szFixed = v->GetSize();
		UISizePolicy sp = v->GetSizePolicy();

		UISize szPref;
		if(sp.cx == UISizePolicy::PREFERRED || sp.cy == UISizePolicy::PREFERRED)
			szPref = v->GetPreferredSize();

		if(sp.cx == UISizePolicy::PREFERRED)
			size.cx = szPref.cx;
		else if(sp.cx == UISizePolicy::EXPANDING)
			size.cx = szPref.cx;
		else
			size.cx = szFixed.cx;

		if(sp.cy == UISizePolicy::PREFERRED)
			size.cy = szPref.cy;
		else if(sp.cy == UISizePolicy::EXPANDING)
			size.cy = szPref.cy;
		else
			size.cy = szFixed.cy;

		rcTemp.SetRect(0, 0, rcMargin.left + size.cx + rcMargin.right, rcMargin.top + size.cy + rcMargin.bottom);
		bound.UnionRect(bound, rcTemp);
	}

	return UISize(bound.Width(), bound.Height());
}

void UICardLayout::OnInit()
{
	if(m_nShownIndex == -1)
		Show(0);

	__super::OnInit();
}

void UICardLayout::OnLayout()
{
	UISize szParent = GetSize();
	UISize size;
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		UISizePolicy sp = v->GetSizePolicy();
		UISize szPref = v->GetPreferredSize();
		UISize szFixed = v->GetSize();
		UIRect rcMargin = v->GetMargin();

		if(sp.cx == UISizePolicy::PREFERRED)
			size.cx = szPref.cx;
		else if(sp.cx == UISizePolicy::EXPANDING)
			size.cx = szParent.cx - rcMargin.left - rcMargin.right;
		else
			size.cx = szFixed.cx;

		if(sp.cy == UISizePolicy::PREFERRED)
			size.cy = szPref.cy;
		else if(sp.cy == UISizePolicy::EXPANDING)
			size.cy = szParent.cy - rcMargin.top - rcMargin.bottom;
		else
			size.cy = szFixed.cy;

		v->SetBounds(rcMargin.left, rcMargin.top, rcMargin.left + size.cx, rcMargin.top + size.cy);
	}

	__super::OnLayout();
}

void UICardLayout::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH);
}