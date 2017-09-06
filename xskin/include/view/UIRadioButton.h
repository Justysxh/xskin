#ifndef __UIRADIO_BUTTON_H_INCLUDED__
#define __UIRADIO_BUTTON_H_INCLUDED__

#include "UIButton.h"

class XSKIN_API UIRadioButton : public UIButton
{
	DECLARE_XML_NODE(UIRadioButton)
	DECLARE_XML_PROPERTY()
public:
	enum { BITMAP_TEXT_SPACING = 3 };

			UIRadioButton(UIView* parent = NULL);
			~UIRadioButton();

public:
	void	SetCheckedBkNormal(const UIBitmap* pBitmap, const UIRect& rcPos);
	void	SetCheckedBkHot(const UIBitmap* pBitmap, const UIRect& rcPos);
	void	SetCheckedBkPushed(const UIBitmap* pBitmap, const UIRect& rcPos);
	void	SetCheckedBkDisabled(const UIBitmap* pBitmap, const UIRect& rcPos);
	void	SetChecked(BOOL b) OVERRIDE;
	void	Activate(BOOL b) OVERRIDE;
	void	SetSpacing(int nSpacing);
	UISize	GetPreferredSize() const OVERRIDE;

protected:
	BOOL	OnActionPerformed(UIActionEvent* event);
	void	OnPaint(HDC hdc) OVERRIDE;

protected:
	int m_nSpacing;
	UIDrawable m_bkCheckedNormal;
	UIDrawable m_bkCheckedHot;
	UIDrawable m_bkCheckedPushed;
	UIDrawable m_bkCheckedDisabled;
};
#endif //! __UIRADIO_BUTTON_H_INCLUDED__