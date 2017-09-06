#include "stdafx.h"
#include "../../include/gfx/UIDrawable.h"
#include "../../include/gfx/UIRender.h"

UIDrawable::UIDrawable()
{
	m_pBitmap = NULL;
	m_rcPos.SetRectEmpty();
}

UIDrawable::UIDrawable(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_pBitmap = pBitmap;
	m_rcPos = rcPos;
}

UIDrawable::~UIDrawable()
{
	if(m_pBitmap)
		m_pBitmap->Release();
}

void UIDrawable::SetBitmap(const UIBitmap* pBitmap)
{
	if(m_pBitmap)
		m_pBitmap->Release();
		
	m_pBitmap = pBitmap;
}

void UIDrawable::SetPos(const UIRect& rcPos)
{
	m_rcPos = rcPos;
}

BOOL UIDrawable::IsValid() const
{
	return m_pBitmap != NULL;
}

int UIDrawable::Width() const
{
	if(m_pBitmap)
	{
		if(m_rcPos.IsRectEmpty())
			return m_pBitmap->GetWidth();
		else
			return m_rcPos.Width();
	}
	else
	{
		return 0;
	}
}

int UIDrawable::Height() const
{
	if(m_pBitmap)
	{
		if(m_rcPos.IsRectEmpty())
			return m_pBitmap->GetHeight();
		else
			return m_rcPos.Height();
	}
	else
	{
		return 0;
	}
}

void UIDrawable::Draw(HDC hdc, const UIRect& rcDest, FillMode fm, int nOpacity)
{
	if(!m_pBitmap)
		return;

	if(m_rcPos.IsRectEmpty())
		m_rcPos.SetRect(0, 0, m_pBitmap->GetWidth(), m_pBitmap->GetHeight());

	if(fm == FILL_NONE)
	{
		UIRect rcPaint(rcDest.left, rcDest.top, rcDest.left + m_rcPos.Width(), rcDest.top + m_rcPos.Height());
		UIRender::DrawBitmap(hdc, rcPaint, m_rcPos, m_pBitmap, nOpacity);
	}
	else if(fm == FILL_SCALE)
	{
		UIRender::DrawBitmap(hdc, rcDest, m_rcPos, m_pBitmap, nOpacity);
	}
	else if(fm == FILL_REPEAT_X)
	{
		if(m_rcPos.Width() > rcDest.Width())
		{
			UIRender::DrawBitmap(hdc, rcDest, m_rcPos, m_pBitmap, nOpacity);
			XTRACE(_T("UIDrawable::Draw() bitmap width is too large."));
		}
		else if(m_rcPos.Width() == rcDest.Width() && m_rcPos.Height() == rcDest.Height())
		{
			UIRender::DrawBitmap(hdc, rcDest, m_rcPos, m_pBitmap, nOpacity);
		}
		else
		{
			XASSERT(m_rcPos.Width() > 3);
			UIRect rcSrc;
			UIRect rcDst;
			int nCenterX = m_rcPos.Width() / 2;

			// left.
			rcSrc.SetRect(m_rcPos.left, m_rcPos.top, m_rcPos.left + nCenterX, m_rcPos.bottom);
			rcDst.SetRect(rcDest.left, rcDest.top, rcDest.left + rcSrc.Width(), rcDest.bottom);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			int h1 = rcDst.right;

			// right.
			rcSrc.SetRect(m_rcPos.right - nCenterX - 1, m_rcPos.top, m_rcPos.right, m_rcPos.bottom);
			rcDst.SetRect(rcDest.right - rcSrc.Width(), rcDest.top, rcDest.right, rcDest.bottom);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			int h2 = rcDst.left;

			// center.
			rcSrc.SetRect(m_rcPos.left + nCenterX, m_rcPos.top, m_rcPos.left + nCenterX + 1, m_rcPos.bottom);
			rcDst.SetRect(h1, rcDest.top, h2, rcDest.bottom);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);
		}
	}
	else if(fm == FILL_REPEAT_Y)
	{
		if(m_rcPos.Height() > rcDest.Height())
		{
			UIRender::DrawBitmap(hdc, rcDest, m_rcPos, m_pBitmap, nOpacity);
			XTRACE(_T("UIDrawable::Draw() bitmap height is too large."));
		}
		else if(m_rcPos.Width() == rcDest.Width() && m_rcPos.Height() == rcDest.Height())
		{
			UIRender::DrawBitmap(hdc, rcDest, m_rcPos, m_pBitmap, nOpacity);
		}
		else
		{
			XASSERT(m_rcPos.Height() > 3);
			UIRect rcSrc;
			UIRect rcDst;
			int nCenterY = m_rcPos.Height() / 2;

			// top.
			rcSrc.SetRect(m_rcPos.left, m_rcPos.top, m_rcPos.right, m_rcPos.top + nCenterY);
			rcDst.SetRect(rcDest.left, rcDest.top, rcDest.right, rcDest.top + rcSrc.Height());
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			int h1 = rcDst.bottom;

			// bottom.
			rcSrc.SetRect(m_rcPos.left, m_rcPos.bottom - nCenterY - 1, m_rcPos.right, m_rcPos.bottom);
			rcDst.SetRect(rcDest.left, rcDest.bottom - rcSrc.Height(), rcDest.right, rcDest.bottom);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			int h2 = rcDst.top;

			// v-center.
			rcSrc.SetRect(m_rcPos.left, m_rcPos.top + nCenterY, m_rcPos.right, m_rcPos.top + nCenterY + 1);
			rcDst.SetRect(rcDest.left, h1, rcDest.right, h2);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);
		}
	}
	else
	{
		if(m_rcPos.Width() > rcDest.Width() || m_rcPos.Height() > rcDest.Height())
		{
			UIRender::DrawBitmap(hdc, rcDest, m_rcPos, m_pBitmap, nOpacity);
			XTRACE(_T("UIDrawable::Draw() bitmap size is too large."));
		}
		else if(m_rcPos.Width() == rcDest.Width() && m_rcPos.Height() == rcDest.Height())
		{
			UIRender::DrawBitmap(hdc, rcDest, m_rcPos, m_pBitmap, nOpacity);
		}
		else
		{
			XASSERT(m_rcPos.Width() > 3);
			XASSERT(m_rcPos.Height() > 3);
			UIRect rcSrc;
			UIRect rcDst;
			UIRect rcCenter;
			int h1, h2, v1, v2;
			int nCenterX = m_rcPos.Width() / 2;
			int nCenterY = m_rcPos.Height() / 2;

			// left-top.
			rcSrc.SetRect(m_rcPos.left, m_rcPos.top, m_rcPos.left + nCenterX, m_rcPos.top + nCenterY);
			rcDst.SetRect(rcDest.left, rcDest.top, rcDest.left + rcSrc.Width(), rcDest.top + rcSrc.Height());
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			h1 = rcDst.right;
			v1 = rcDst.bottom;
			rcCenter.left = rcDst.right;
			rcCenter.top = rcDst.bottom;

			// right-top.
			rcSrc.SetRect(m_rcPos.right - nCenterX - 1, m_rcPos.top, m_rcPos.right, m_rcPos.top + nCenterY);
			rcDst.SetRect(rcDest.right - rcSrc.Width(), rcDest.top, rcDest.right, rcDest.top + rcSrc.Height());
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			h2 = rcDst.left;
			v1 = rcDst.bottom;
			rcCenter.right = rcDst.left;

			// h-center-top
			rcSrc.SetRect(m_rcPos.left + nCenterX, m_rcPos.top, m_rcPos.left + nCenterX + 1, m_rcPos.top + nCenterY);
			rcDst.SetRect(h1, rcDest.top, h2, rcDest.top + nCenterY);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			// left-bottom.
			rcSrc.SetRect(m_rcPos.left, m_rcPos.bottom - nCenterY - 1, m_rcPos.left + nCenterX, m_rcPos.bottom);
			rcDst.SetRect(rcDest.left, rcDest.bottom - rcSrc.Height(), rcDest.left + rcSrc.Width(), rcDest.bottom);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			h1 = rcDst.right;
			v2 = rcDst.top;
			rcCenter.bottom = rcDst.top;

			// v-center-left.
			rcSrc.SetRect(m_rcPos.left, m_rcPos.top + nCenterY, m_rcPos.left + nCenterX, m_rcPos.top + nCenterY + 1);
			rcDst.SetRect(rcDest.left, v1, rcDest.left + rcSrc.Width(), v2);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			// right-bottom.
			rcSrc.SetRect(m_rcPos.right - nCenterX - 1, m_rcPos.bottom - nCenterY - 1, m_rcPos.right, m_rcPos.bottom);
			rcDst.SetRect(rcDest.right - rcSrc.Width(), rcDest.bottom - rcSrc.Height(), rcDest.right, rcDest.bottom);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			h2 = rcDst.left;
			v2 = rcDst.top;

			// h-center-bottom.
			rcSrc.SetRect(m_rcPos.left + nCenterX, m_rcPos.bottom - nCenterY - 1, m_rcPos.left + nCenterX + 1, m_rcPos.bottom);
			rcDst.SetRect(h1, rcDest.bottom - nCenterY - 1, h2, rcDest.bottom);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			// v-center-right.
			rcSrc.SetRect(m_rcPos.right - nCenterX - 1, m_rcPos.top + nCenterY, m_rcPos.right, m_rcPos.top + nCenterY + 1);
			rcDst.SetRect(rcDest.right - rcSrc.Width(), v1, rcDest.right, v2);
			UIRender::DrawBitmap(hdc, rcDst, rcSrc, m_pBitmap, nOpacity);

			// center
			rcSrc.SetRect(m_rcPos.left + nCenterX, m_rcPos.top + nCenterY, m_rcPos.left + nCenterX + 1, m_rcPos.top + nCenterY + 1);
			UIRender::DrawBitmap(hdc, rcCenter, rcSrc, m_pBitmap, nOpacity);
		}
	}
}