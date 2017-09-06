#ifndef __UIMEASURE_H_INCLUDED__
#define __UIMEASURE_H_INCLUDED__

enum Direction
{
	HORIZONTAL,
	VERTICAL
};

class XSKIN_API UISizePolicy
{
public:
	enum Policy
	{
		FIXED = 0,
		PREFERRED,
		EXPANDING,
	};

	inline UISizePolicy()
	{
		cx = cy = FIXED;
	}

	inline UISizePolicy(Policy horizontal, Policy vertical)
	{
		cx = horizontal;
		cy = vertical;
	}

	inline UISizePolicy(const UISizePolicy& src)
	{
		cx = src.cx;
		cy = src.cy;
	}

	inline ~UISizePolicy()
	{

	}

public:
	Policy cx;
	Policy cy;
};

class XSKIN_API UIAlignment
{
public:
	enum AlignFlag
	{
		ALIGN_LEFT = 0,
		ALIGN_HCENTER,
		ALIGN_RIGHT,
		ALIGN_TOP,
		ALIGN_VCENTER,
		ALIGN_BOTTOM
	};

	inline UIAlignment()
	{
		cx = ALIGN_LEFT;
		cy = ALIGN_TOP;
	}

	inline UIAlignment(AlignFlag horz, AlignFlag vert)
	{
		cx = horz;
		cy = vert;
	}

	inline UIAlignment(const UIAlignment& src)
	{
		cx = src.cx;
		cy = src.cy;
	}

	inline ~UIAlignment()
	{

	}

public:
	inline operator int()
	{
		return cx | cy;
	}

	inline operator int() const
	{
		return cx | cy;
	}

public:
	AlignFlag cx;
	AlignFlag cy;
};
#endif //! __UIMEASURE_H_INCLUDED__