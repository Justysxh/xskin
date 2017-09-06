#ifndef __UIIMAGE_BUTTON_H_INCLUDED__
#define __UIIMAGE_BUTTON_H_INCLUDED__

#include "UIButton.h"
#include "../gfx/UIDrawable.h"

class XSKIN_API UIImageButton : public UIButton
{
	DECLARE_XML_NODE(UIImageButton)
	DECLARE_XML_PROPERTY()
public:
	enum { BITMAP_TEXT_SPACING = 8 };

			UIImageButton(UIView* parent = NULL);
			~UIImageButton();

public:
	void	SetIcon(const UIBitmap* pBitmap, const UIRect& rcPos);
	void	SetSpacing(int nSpacing);
	UISize	GetPreferredSize() const OVERRIDE;

protected:
	void	OnPaint(HDC hdc) OVERRIDE;

protected:
	UIDrawable m_bkIcon;
	int m_nSpacing;
};

#endif //! __UIIMAGE_BUTTON_H_INCLUDED___