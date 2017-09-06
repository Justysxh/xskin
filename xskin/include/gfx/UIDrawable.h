#ifndef __UIDRAWABLE_H_INCLUDED__
#define __UIDRAWABLE_H_INCLUDED__

#include "../core/UIObject.h"
#include "../core/UISize.h"
#include "../core/UIRect.h"
#include "UIBitmap.h"

class XSKIN_API UIDrawable : public UIObject
{
public:
	enum FillMode
	{
		FILL_NONE = 0,
		FILL_SCALE,
		FILL_REPEAT_X,
		FILL_REPEAT_Y,
		FILL_REPEAT_BOTH
	};

					UIDrawable();
					UIDrawable(const UIBitmap* pBitmap, const UIRect& rcPos);
					~UIDrawable();

public:
	BOOL			IsValid() const;
	int				Width() const;
	int				Height() const;
	void			SetBitmap(const UIBitmap* pBitmap);
	void			SetPos(const UIRect& rcPos);
	void			Draw(HDC hdc, const UIRect& rcDest, FillMode fm = FILL_SCALE, int nOpacity = 255);

private:
	const UIBitmap* m_pBitmap;
	UIRect m_rcPos;
};
#endif //! __UIBACKGROUND_H_INCLUDED__