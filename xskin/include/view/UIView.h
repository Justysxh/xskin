#ifndef __UIVIEW_H_INCLUDED__
#define __UIVIEW_H_INCLUDED__

#include <string>
#include <vector>
#include <Windows.h>
#include "../../include/core/UIResponser.h"
#include "../../include/core/UIApplication.h"
#include "../../include/core/UIString.h"
#include "../../include/core/UIPoint.h"
#include "../../include/core/UIRect.h"
#include "../../include/core/UISize.h"
#include "../../include/core/UISizePolicy.h"
#include "../../include/core/UIMeasure.h"
#include "../../include/core/UIAsset.h"
#include "../../include/gfx/UIDrawable.h"

typedef std::vector<UIView*> ViewList;
typedef struct _ExtendedLayoutProps
{
	/// For UIGridLayout only.
	int nRow;
	int nColumn;
	int nRowSpan;
	int nColumnSpan;
	UIAlignment alignment;
} ExtendedLayoutParams;

namespace tinyxml2 { class XSKIN_API XMLElement; };
class UIEvent;
class UIMouseEvent;
class UIWheelEvent;
class UIKeyEvent;
class UIFocusEvent;
class UITimerEvent;
class UIActionEvent;
class UIRootView;
class UIBitmap;
class UIAnimation;
class UIRootView;
class XSKIN_API UIView : public UIResponser
{
	DECLARE_XML_NODE(UIView)
public:
								UIView(UIView* parent = NULL);
								~UIView();

public:
	void						SetBounds(int l, int t, int r, int b);
	void						SetBounds(const UIRect& rc);
	const UIRect&				GetBounds() const;
	void						SetMargin(int l, int t, int r, int b);
	void						SetMargin(const UIRect& rc);
	const UIRect&				GetMargin() const;
	void						SetPadding(int l, int t, int r, int b);
	void						SetPadding(const UIRect& rc);
	const UIRect&				GetPadding() const;
	void						SetSize(int cx, int cy);
	void						SetSize(const UISize& sz);
	UISize						GetSize() const;
	void						SetPosition(const UIPoint& pt);
	void						SetPosition(int x, int y);
	UIPoint						GetPosition() const;
	virtual void				SetVisible(BOOL b);
	virtual BOOL				IsVisible() const;
	virtual void				SetEnabled(BOOL b);
	virtual BOOL				IsEnabled() const;
	virtual void				SetChecked(BOOL b);
	virtual BOOL				IsChecked() const;
	virtual BOOL				IsFocusFrameVisible() const;
	virtual void				Activate(BOOL b);
	void						SetBorderVisible(BOOL b);
	BOOL						IsBorderVisible() const;
	void						SetOverlayVisible(BOOL b);
	void						SetBorderColor(COLORREF clrFrame);
	COLORREF					GetBorderColor() const;
	BOOL						IsActivated() const;
	INT_PTR						GetTag() const;
	void						SetTag(INT_PTR nTag);
	void						SetID(const UIString& str);
	UIString					GetID() const;
	void						SetGroup(const UIString& str);
	UIString					GetGroup() const;
	void						SetSizePolicy(UISizePolicy::Policy horz, UISizePolicy::Policy vert);
	const UISizePolicy&			GetSizePolicy() const;
	void						SetBkgnd(const UIBitmap* pBitmap, const UIRect& rcPos);
	void						SetSkin(const UIString& szSkinXmlFile);
	UIString					GetSkin() const;
	UINT						StartTimer(UINT_PTR uTimerID, int nInterval);
	void						KillTimer(UINT nTimerID);
	BOOL						HitTest(const UIPoint& pt) const;
	BOOL						HitTest(int x, int y) const;
	void						SetCursor(HCURSOR hCursor);
	HCURSOR						GetCursor() const;
	void						SetTooltipText(const UIString& szText);
	UIString					GetTooltipText() const;
	void						SetParent(UIView* v);
	UIView*						GetParent() const;
	void						SetClipRect(const UIRect& rcClip);
	void						SetClipRect(int l, int t, int r, int b);
	const UIRect&				GetClipRect() const;
	BOOL						IsParentOf(UIView* v) const;
	virtual void				SetAttribute(const UIString& szAttrName, const UIString& szAttrValue);
	virtual BOOL				OnCustomAttribute(const UIString& szAttrName, const UIString& szAttrValue);
	virtual UISize				GetPreferredSize() const;
	virtual UISize				GetMinimumSize() const;
	virtual UISize				GetMaximumSize() const;
	void						SetViewportOrg(int x, int y);
	const UIPoint&				GetViewportOrg() const;
	BOOL						HasFocus();
	void						RequestFocus();
	void						ClearFocus();
	void						SetOpacity(int nOpacity);
	int							GetOpacity() const;
	void						SetFocusable(BOOL b);
	BOOL						IsFocusable() const;
	BOOL						IsInitialized() const;
	void						SetTabIndex(int nIndex);
	int							GetTabIndex() const;
	UIView*						GetViewForID(const UIString& id);
	virtual UIView*				GetViewForPoint(const UIPoint& pt);
	UIView*						GetSelectedViewForGroup(const UIString& szGroup);
	void						GetViewsWithGroup(const UIString& szGroup, std::vector<UIView*>* pOut);
	void						GetViewsWithTabstop(std::vector<UIView*>* pOut);
	UIView*						GetNextFocusableView();
	UIView*						GetPrevFocusableView();
	void						SetNextFocusableView(UIView* v);
	void						SetPrevFocusableView(UIView* v);
	virtual UIRootView*			GetRootView();
	void						AddChildView(UIView* v);
	void						AddChildView(UIView* v, int nIndex);
	UIView*						RemoveChildView(UIView* v);
	UIView*						RemoveChildView(int nIndex);
	void						RemoveAllChildView();
	int							GetChildViewCount() const;
	UIView*						GetChildView(int nIndex) const;
	void						SetExtendedLayoutParams(const ExtendedLayoutParams& props);
	const ExtendedLayoutParams&	GetExtendedLayoutParams() const;
	void						SetAnimation(UIAnimation* pAnimation);
	UIAnimation*				GetAnimation();

	void						RequestLayout();
	void						InvalidateLayout();
	void						Invalidate();
	static UIPoint				MapToWindow(UIView* pView, const UIPoint& pt);
	static UIRect				MapToWindow(UIView* pView, const UIRect& rc);
	static UIPoint				MapToScreen(UIView* pView, const UIPoint& pt);
	static UIRect				MapToScreen(UIView* pView, const UIRect& rc);
	static UIPoint				MapToView(UIView* pView, const UIPoint& pt);
	static UIRect				MapToView(UIView* pView, const UIRect& rc);
	static UIView*				Inflate(const UIString& szXmlFileName, UIView* pAttachTo = NULL);

protected:
	UIResponser*				GetNextResponser() const OVERRIDE;
	virtual BOOL				Event(UIEvent* event) OVERRIDE;
	virtual void				OnInit();
	virtual void				OnLayout();
	virtual void				OnPaint(HDC hdc);
	void						DoPaint(HDC hdc, const UIRect& rcDirty);
	static UIView*				InflateFromXML(const tinyxml2::XMLElement* pNode);

	virtual BOOL				OnMouseDblClicked(UIMouseEvent* event);
	virtual BOOL				OnMousePressed(UIMouseEvent* event);
	virtual BOOL				OnMouseDragged(UIMouseEvent* event);
	virtual BOOL				OnMouseReleased(UIMouseEvent* event);
	virtual BOOL				OnMouseMoved(UIMouseEvent* event);
	virtual BOOL				OnMouseEntered(UIMouseEvent* event);
	virtual BOOL				OnMouseExited(UIMouseEvent* event);
	virtual BOOL				OnMouseClicked(UIMouseEvent* event);
	virtual BOOL				OnMouseWheel(UIWheelEvent* event);
	virtual BOOL				OnKeyPressed(UIKeyEvent* event);
	virtual BOOL				OnKeyReleased(UIKeyEvent* event);
	virtual BOOL				OnFocusGain(UIFocusEvent* event);
	virtual BOOL				OnFocusLost(UIFocusEvent* event);
	virtual BOOL				OnTimer(UITimerEvent* event);
	virtual	BOOL				OnActionPerformed(UIActionEvent* event);

protected:
	UIString m_szID;
	UIString m_szGroup;
	UIString m_szTooltip;
	UIString m_szSkinXmlFile;
	HCURSOR m_hCursor;
	INT_PTR m_nTag;
	UIRect m_rcBounds;
	UIRect m_rcMargin;
	UIRect m_rcPadding;
	UIRect m_rcClip;
	UISizePolicy m_szPolicy;
	UIDrawable m_bkgnd;
	int m_nOpacity;
	int m_nTabIndex;
	COLORREF m_clrFrame;
	BOOL m_bBorderVisible;
	BOOL m_bVisible;
	BOOL m_bFocusable;
	BOOL m_bEnabled;
	BOOL m_bChecked;
	BOOL m_bNeedLayout;
	BOOL m_bActivated;
	BOOL m_bInitialized;
	UIRootView* m_pRootView;
	UIView* m_pParent;
	UIView* m_pPrevFocusableView;
	UIView* m_pNextFocusableView;
	UIPoint m_ptViewportOrg;
	ViewList m_childViews;
	ExtendedLayoutParams m_params;
	UIAnimation* m_pAnimation;
	static BOOL g_bOverlayVisible;
};
#endif //! __UIVIEW_H_INCLUDED__