#ifndef __H_INCLUDED__
#define __H_INCLUDED__

#include <Windows.h>

class XSKIN_API UISize : public tagSIZE
{
public:
	inline UISize()
	{
		cx = cy = 0;
	}

	inline UISize(const SIZE& src)
	{
		cx = src.cx;
		cy = src.cy;
	}

	inline UISize(const RECT rc)
	{
		cx = rc.right - rc.left;
		cy = rc.bottom - rc.top;
	}

	inline UISize(int cx, int cy)
	{
		this->cx = cx;
		this->cy = cy;
	}

	inline ~UISize()
	{

	}

public:
	inline operator LPSIZE()
	{
		return this;
	}
};
#endif //! __H_INCLUDED__
