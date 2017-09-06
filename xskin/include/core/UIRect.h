#ifndef __UIRECT_H_INCLUDED__
#define __UIRECT_H_INCLUDED__

#include <Windows.h>

class XSKIN_API UIRect : public tagRECT
{
public:
	inline UIRect()
	{
		left = top = right = bottom = 0;
	}

	inline UIRect(const RECT& src)
	{
		left = src.left;
		top = src.top;
		right = src.right;
		bottom = src.bottom;
	}

	inline UIRect(int l, int t, int r, int b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	inline UIRect(POINT point, SIZE size)
	{
		right = (left = point.x) + size.cx;
		bottom = (top = point.y) + size.cy;
	}

	inline ~UIRect()
	{

	}

public:
	inline int	Width() const
	{
		return right - left;
	}

	inline int Height() const
	{
		return bottom - top;
	}

	inline void	SetWidth(int cx)
	{
		right = left + cx;
	}

	inline void	SetHeight(int cy)
	{
		bottom = top + cy;
	}

	inline void	SetSize(SIZE sz)
	{
		right = left + sz.cx;
		bottom = top + sz.cy;
	}

	inline SIZE	GetSize() const
	{
		SIZE size;
		size.cx = right - left;
		size.cy = bottom - left;

		return size;
	}

	inline void	SetPos(int x, int y)
	{
		int nWidth = right - left;
		int nHeight = bottom - top;
		left = x;
		top = y;
		right = left + nWidth;
		bottom = top + nHeight;
	}

	inline POINT GetPos() const
	{
		POINT pt;
		pt.x = left;
		pt.y = top;

		return pt;
	}

	inline void SetRect(int l, int t, int r, int b)
	{
		left = l; top = t; right = r; bottom = b;
	}

	inline void	SetRectEmpty()
	{
		left = top = right = bottom = 0;
	}

	inline BOOL	IsRectEmpty() const
	{
		return ::IsRectEmpty(this);
	}

	inline BOOL	IntersectRect(LPCRECT rc1, LPCRECT rc2)
	{
		return ::IntersectRect(this, rc1, rc2);
	}

	inline BOOL	UnionRect(LPCRECT rc1, LPCRECT rc2)
	{
		return ::UnionRect(this, rc1, rc2);
	}

	inline void	NormalizeRect()
	{
		int nTemp;
		if(left > right)
		{
			nTemp = left;
			left = right;
			right = nTemp;
		}
		if(top > bottom)
		{
			nTemp = top;
			top = bottom;
			bottom = nTemp;
		}
	}

	inline void	OffsetRect(int cx, int cy)
	{
		::OffsetRect(this, cx, cy);
	}

	inline void	InflateRect(int cx, int cy)
	{
		::InflateRect(this, cx, cy);
	}

	inline void	DeflateRect(int cx, int cy)
	{
		::InflateRect(this, -cx, -cy);
	}

	inline BOOL	PtInRect(const POINT& point) const
	{
		return ::PtInRect(this, point);
	}

	inline operator LPRECT()
	{
		return this;
	}

	inline operator LPCRECT() const
	{
		return this;
	}

	inline BOOL operator ==(const UIRect& rhs) const
	{
		return left == rhs.left && top == rhs.top && 
			right == rhs.right && bottom == rhs.bottom;
	}
};
#endif //! __UIRECT_H_INCLUDED__