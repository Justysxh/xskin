#ifndef __UISCROLL_BAR_H_INCLUDED__
#define __UISCROLL_BAR_H_INCLUDED__

#include "UIButton.h"
#include "../gfx/UIDrawable.h"

class XSKIN_API UIScrollBar : public UIView
{
protected:
	class XSKIN_API UIScrollButton : public UIControl
	{
	public:
		enum { AUTO_SCROLL_DELAY_MS = 500 };

					UIScrollButton(UIView* parent = NULL);
					~UIScrollButton();
		BOOL		EnsureDelay();
		BOOL		IsFocusable() const OVERRIDE;
		UISize		GetPreferredSize() const OVERRIDE;

	protected:
		void		OnPaint(HDC hdc) OVERRIDE;
		BOOL		OnMousePressed(UIMouseEvent* event) OVERRIDE;
		BOOL		OnMouseReleased(UIMouseEvent* event) OVERRIDE;
		BOOL		OnMouseDragged(UIMouseEvent* event) OVERRIDE;
		BOOL		OnMouseWheel(UIWheelEvent* event) OVERRIDE;

	private:
		UINT_PTR m_nTimerID;
		DWORD m_nTickStart;
	};

	DECLARE_XML_NODE(UIScrollBar)
	DECLARE_XML_PROPERTY()
public:
					UIScrollBar(UIView* parent = NULL);
					~UIScrollBar();

public:
	void			Scroll(int nDistance);
	void			SetPos(int nPos);
	int				GetPos() const;
	void			LineUp();
	void			LineDown();
	void			PageUp();
	void			PageDown();
	void			WheelUp();
	void			WheelDown();
	void			SetViewportSize(int nSize);
	int				GetViewportSize() const;
	void			SetLineSize(int nLineSize);
	int				GetLineSize() const;
	void			SetRange(int nMin, int nMax);
	UISize			GetRange() const;
	void			SetDirection(Direction orient);
	Direction		GetDirection() const;
	UISize			GetPreferredSize() const OVERRIDE;

protected:
	void			SetTrackPos(const UIRect& rcTrack);
	void			UpdateTrackPos();
	int				Clamp(int x, int a, int b);
	void			OnLayout() OVERRIDE;
	void			OnPaint(HDC hdc) OVERRIDE;
	BOOL			OnMousePressed(UIMouseEvent* event) OVERRIDE;
	BOOL			OnMouseReleased(UIMouseEvent* event) OVERRIDE;
	BOOL			OnMouseWheel(UIWheelEvent* event) OVERRIDE;
	BOOL			OnMouseDragged(UIMouseEvent* event) OVERRIDE;
	BOOL			OnTimer(UITimerEvent* event) OVERRIDE;
	BOOL			OnActionPerformed(UIActionEvent* event) OVERRIDE;

protected:
	Direction m_direction;
	UIScrollButton* m_pLineUp;
	UIScrollButton* m_pLineDown;
	UIScrollButton* m_pTrack;
	UIPoint m_ptLast;
	int m_nViewportSize;
	int m_nLineSize;
	int m_nMin;
	int m_nMax;
	int m_nPos;
	int m_nMinTrackSize;
};
#endif //! __UISCROLL_BAR_H_INCLUDED__