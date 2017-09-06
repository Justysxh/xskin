#ifndef __UIPOINT_H_INCLUDED__
#define __UIPOINT_H_INCLUDED__

#include <Windows.h>

class XSKIN_API UIPoint : public tagPOINT
{
public:
	inline UIPoint()
	{
		x = y = 0;
	}

	inline UIPoint(const POINT& src)
	{
		x = src.x;
		y = src.y;
	}

	inline UIPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	inline UIPoint(LPARAM lParam)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
	}

	inline ~UIPoint()
	{

	}

public:
	inline void SetPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	inline void	Offset(int xOffset, int yOffset)
	{
		x += xOffset;
		y += yOffset;
	}

	inline UIPoint operator+(POINT point) const
	{
		return UIPoint(x + point.x, y + point.y);
	}

	inline UIPoint operator-(POINT point) const
	{
		return UIPoint(x - point.x, y - point.y);
	}

	inline operator LPPOINT()
	{
		return this;
	}
};
#endif //! __UIPOINT_H_INCLUDED__