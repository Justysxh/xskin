#include "stdafx.h"
#include "../../include/gfx/UIRender.h"

UIRender::UIRender()
{

}

UIRender::~UIRender()
{

}

void UIRender::DrawLine(HDC hdc, const UIPoint& p1, const UIPoint& p2)
{
	::MoveToEx(hdc, p1.x, p1.y, NULL);
	::LineTo(hdc, p2.x, p2.y);
}

void UIRender::DrawString(HDC hdc, const UIRect& rc, const UIString& szText, const UIAlignment& align, BOOL bMultiline)
{
	int nFlags = DT_END_ELLIPSIS;

	if(bMultiline)
		nFlags |= (DT_WORDBREAK | DT_EDITCONTROL);
	else
		nFlags |= DT_SINGLELINE;

	if(align.cx == UIAlignment::ALIGN_LEFT)
		nFlags |= DT_LEFT;
	else if(align.cx == UIAlignment::ALIGN_HCENTER)
		nFlags |= DT_CENTER;
	else
		nFlags |= DT_RIGHT;

	if(align.cy == UIAlignment::ALIGN_TOP)
		nFlags |= DT_TOP;
	else if(align.cy == UIAlignment::ALIGN_VCENTER)
		nFlags |= DT_VCENTER;
	else
		nFlags |= DT_BOTTOM;

	int nOldMode = ::SetBkMode(hdc, TRANSPARENT);
	UIRect rcTemp = rc;

	DrawText(hdc, szText, -1, rcTemp, nFlags);
	::SetBkMode(hdc, nOldMode);
}

void UIRender::MeasureString(HDC hdc, UIRect& rcOut, const UIString& szText, const UIAlignment& align, BOOL bMultiline)
{
	int nFlags = DT_CALCRECT;

	if(bMultiline)
		nFlags |= (DT_WORDBREAK | DT_EDITCONTROL);
	else
		nFlags |= DT_SINGLELINE;

	if(align.cx == UIAlignment::ALIGN_LEFT)
		nFlags |= DT_LEFT;
	else if(align.cx == UIAlignment::ALIGN_HCENTER)
		nFlags |= DT_CENTER;
	else
		nFlags |= DT_RIGHT;

	if(align.cy == UIAlignment::ALIGN_TOP)
		nFlags |= DT_TOP;
	else if(align.cy == UIAlignment::ALIGN_VCENTER)
		nFlags |= DT_VCENTER;
	else
		nFlags |= DT_BOTTOM;

	::DrawText(hdc, szText, -1, rcOut, nFlags);
}

void UIRender::DrawRect(HDC hdc, const UIRect& rc, const UIBrush& hbr)
{
	::FrameRect(hdc, rc, hbr);
}

void UIRender::DrawFocusRect(HDC hdc, const UIRect& rc)
{
	::DrawFocusRect(hdc, rc);
}

void UIRender::FillRect(HDC hdc, const UIRect& rc, const UIBrush& hbr)
{
	::FillRect(hdc, rc, hbr);
}

void UIRender::DrawBitmap(HDC hdc, const UIRect& dst, const UIRect& src, const UIBitmap* pBitmap, int nOpacity)
{
	HDC hMemDC = ::CreateCompatibleDC(NULL);
	::SelectObject(hMemDC, *pBitmap);
	if(pBitmap->HasAlpha())
	{
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.SourceConstantAlpha = nOpacity;
		::AlphaBlend(hdc, dst.left, dst.top, dst.Width(), dst.Height(), hMemDC, src.left, src.top, src.Width(), src.Height(), bf);
	}
	else
	{
		::SetStretchBltMode(hdc, COLORONCOLOR);
		::StretchBlt(hdc, dst.left, dst.top, dst.Width(), dst.Height(), hMemDC, src.left, src.top, src.Width(), src.Height(), SRCCOPY);
	}
	::DeleteDC(hMemDC);
}

void UIRender::DrawBitmap(HDC hdc, const UIRect& dst, const UIBitmap* pBitmap, int nOpacity)
{
	UIRect rcBmp(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	DrawBitmap(hdc, dst, rcBmp, pBitmap, nOpacity);
}