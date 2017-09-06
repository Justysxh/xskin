#ifndef __UISCROLL_VIEW_H_INCLUDED__
#define __UISCROLL_VIEW_H_INCLUDED__

#include "UIView.h"

class UIScrollBar;
class XSKIN_API UIScrollView : public UIView
{
	DECLARE_XML_NODE(UIScrollView)
	DECLARE_XML_PROPERTY()
public:
					UIScrollView(UIView* parent = NULL);
					~UIScrollView();

public:
	void			SetContentView(UIView* v);
	UIView*			GetContentView();
	void			RemoveContentView();
	void			SetScrollUnit(int nUint);
	void			ScrollToTop();
	void			ScrollToBottom();
	void			ScrollToLeft();
	void			ScrollToRight();
	UIPoint			GetOffset() const;
	UISize			GetPageSize() const;
	UISize			GetPreferredSize() const OVERRIDE;
	BOOL			IsFocusFrameVisible() const OVERRIDE;

protected:
	virtual void	ProcessScrollbar();
	virtual void	ProcessContent();
	virtual	void	Scroll(int x, int y);
	void			OnLayout() OVERRIDE;
	void			OnInit() OVERRIDE;
	void			OnPaint(HDC hdc) OVERRIDE;
	BOOL			OnMouseWheel(UIWheelEvent* event) OVERRIDE;
	BOOL			OnKeyPressed(UIKeyEvent* event) OVERRIDE;
	BOOL			OnActionPerformed(UIActionEvent* event) OVERRIDE;

protected:
	UIScrollBar* m_pHScrollBar;
	UIScrollBar* m_pVScrollBar;
	BOOL m_bHScrollVisible;
	BOOL m_bVScrollVisible;
	UIPoint m_ptOffset;
	UISize m_szPage;
};
#endif //! __UISCROLL_VIEW_H_INCLUDED__