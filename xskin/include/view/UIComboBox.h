#ifndef __UICOMBO_BOX_H_INCLUDED__
#define __UICOMBO_BOX_H_INCLUDED__

#include "UIControl.h"

class UIModel;
class UIListView;
class UIPanel;
class XSKIN_API UIComboBox : public UIControl
{
	DECLARE_XML_NODE(UIComboBox)
	DECLARE_XML_PROPERTY()
public:
				UIComboBox(UIView* parent = NULL);
				~UIComboBox();

public:
	void		SetModel(UIModel* model);
	UIModel*	GetModel() const;
	UIView*		GetItem(int nIndex) const;
	void		SetCheckedIndex(int nIndex);
	void		SetCheckedItem(UIView* v);
	UIView*		GetCheckedItem() const;
	int			GetCheckedIndex() const;
	void		SetDroplistHeight(int nHeight);
	void		Activate(BOOL b) OVERRIDE;
	BOOL		IsFocusFrameVisible() const OVERRIDE;

protected:
	void		OnInit() OVERRIDE;
	void		OnPaint(HDC hdc) OVERRIDE;
	BOOL		OnMouseWheel(UIWheelEvent* event) OVERRIDE;
	BOOL		OnKeyPressed(UIKeyEvent* event) OVERRIDE;
	BOOL		OnKeyReleased(UIKeyEvent* event) OVERRIDE;
	BOOL		OnFocusGain(UIFocusEvent* event) OVERRIDE;
	BOOL		OnFocusLost(UIFocusEvent* event) OVERRIDE;
	BOOL		OnMousePressed(UIMouseEvent* event) OVERRIDE;
	BOOL		OnActionPerformed(UIActionEvent* event) OVERRIDE;

protected:
	UIListView* m_pListView;
	UIPanel* m_pPanel;
	int m_nDroplistHeight;
};
#endif //! __UICOMBO_BOX_H_INCLUDED__