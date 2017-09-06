#ifndef __UICONTROL_H_INCLUDED__
#define __UICONTROL_H_INCLUDED__

#include "UIView.h"

class XSKIN_API UIControl : public UIView
{
	DECLARE_XML_NODE(UIControl)
	DECLARE_XML_PROPERTY()
public:
	enum BackgroundState
	{
		BS_NORMAL = 0,
		BS_HOT,
		BS_PUSHED,
		BS_DISABLED
	};
						UIControl(UIView* parent = NULL);
						~UIControl();

public:
	void				SetText(const UIString& szText);
	UIString			GetText() const;
	void				SetFont(const UIFont* pFont);
	const UIFont*		GetFont() const;
	void				SetTextColor(COLORREF clr);
	COLORREF			GetTextColor() const;
	void				SetDisabledTextColor(COLORREF clr);
	COLORREF			GetDisabledTextColor() const;
	void				SetTextAlign(UIAlignment::AlignFlag cx, UIAlignment::AlignFlag cy);
	UIAlignment			GetTextAlign() const;
	void				SetBkState(BackgroundState state);
	BackgroundState		GetBkState() const;
	void				SetBkNormal(const UIBitmap* pBitmap, const UIRect& rcPos);
	void				SetBkHot(const UIBitmap* pBitmap, const UIRect& rcPos);
	void				SetBkPushed(const UIBitmap* pBitmap, const UIRect& rcPos);
	void				SetBkDisabled(const UIBitmap* pBitmap, const UIRect& rcPos);
	void				SetBkChecked(const UIBitmap* pBitmap, const UIRect& rcPos);
	UIDrawable*			GetBkNormal();
	UIDrawable*			GetBkHot();
	UIDrawable*			GetBkPushed();
	UIDrawable*			GetBkDisabled();
	UIDrawable*			GetBkChecked();
	void				SetEnabled(BOOL b) OVERRIDE;
	UISize				GetPreferredSize() const OVERRIDE;

protected:
	void				OnInit() OVERRIDE;
	BOOL				OnMousePressed(UIMouseEvent* event) OVERRIDE;
	BOOL				OnMouseReleased(UIMouseEvent* event) OVERRIDE;
	BOOL				OnMouseEntered(UIMouseEvent* event) OVERRIDE;
	BOOL				OnMouseExited(UIMouseEvent* event) OVERRIDE;
	BOOL				OnKeyPressed(UIKeyEvent* event) OVERRIDE;
	BOOL				OnKeyReleased(UIKeyEvent* event) OVERRIDE;

protected:
	UIString m_szText;
	const UIFont* m_pFont;
	UIAlignment m_alignment;
	COLORREF m_clrText;
	COLORREF m_clrTextDisabled;
	BackgroundState	m_bkState;
	UIDrawable m_bkNormal;
	UIDrawable m_bkHot;
	UIDrawable m_bkPushed;
	UIDrawable m_bkDisabled;
	UIDrawable m_bkChecked;
};

#endif //! __UICONTROL_H_INCLUDED__