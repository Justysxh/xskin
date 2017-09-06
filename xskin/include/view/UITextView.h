#ifndef __UITEXT_VIEW_H_INCLUDED__
#define __UITEXT_VIEW_H_INCLUDED__

#include "UIControl.h"

class UIScrollBar;
class XSKIN_API UITextView : public UIControl
{
	DECLARE_XML_NODE(UITextView)
	DECLARE_XML_PROPERTY()
public:
						UITextView(UIView* parent = NULL);
						~UITextView();

public:
	UISize				GetPreferredSize() const OVERRIDE;

protected:
	void				OnLayout() OVERRIDE;
	void				OnPaint(HDC hdc) OVERRIDE;
	BOOL				OnMouseWheel(UIWheelEvent* event) OVERRIDE;
	BOOL				OnActionPerformed(UIActionEvent* event) OVERRIDE;

private:
	UIScrollBar* m_pVScrollbar;
	int m_nScroll;
	BOOL m_bMultiline;
};
#endif //! __UITEXT_VIEW_H_INCLUDED__