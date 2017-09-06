#include "stdafx.h"
#include "../../../tinyxml2/tinyxml2.h"
#include "../../include/view/UIView.h"
#include "../../include/view/UIRootView.h"
#include "../../include/view/UIWindow.h"
#include "../../include/core/UIAsset.h"
#include "../../include/core/UIApplication.h"
#include "../../include/gfx/UIBitmap.h"
#include "../../include/gfx/UIRender.h"
#include "../../include/event/UIEvent.h"
#include "../../include/event/UIMouseEvent.h"
#include "../../include/event/UIWheelEvent.h"
#include "../../include/event/UIKeyEvent.h"
#include "../../include/event/UIFocusEvent.h"
#include "../../include/event/UITimerEvent.h"
#include "../../include/event/UIActionEvent.h"
#include "../../include/animation/UIAlphaAnimation.h"

_XMLNodeInfo* _XMLNodeInfo::pFirst = NULL;
_XMLNodeInfo::_XMLNodeInfo(LPCTSTR cls_name, CreateView pfn)
	: szClassName(cls_name)
	, pFunc(pfn)
	, pNext(_XMLNodeInfo::pFirst)
{
	_XMLNodeInfo::pFirst = this;
}

IMPLEMENT_XML_NODE(UIView)

UIView* _XMLNodeInfo::CreateNode(LPCTSTR szNodeName)
{
	_XMLNodeInfo* p = _XMLNodeInfo::pFirst;
	while(p)
	{
		if(_tcscmp(p->szClassName, szNodeName) == 0)
			return (*p->pFunc)();

		p = p->pNext;
	}

	return NULL;
}


BOOL UIView::g_bOverlayVisible = FALSE;

UIView::UIView(UIView* parent)
{
	m_rcBounds.SetRectEmpty();
	m_szID = _T("");
	m_szGroup = _T("");
	m_szTooltip = _T("");
	m_nTag = NULL;
	m_nOpacity = 255;
	m_nTabIndex = 0;
	m_bVisible = TRUE;
	m_bFocusable = TRUE;
	m_bActivated = FALSE;
	m_bEnabled = TRUE;
	m_bNeedLayout = TRUE;
	m_bChecked = FALSE;
	m_bInitialized = FALSE;
	m_clrFrame = RGB(0, 0, 0);
	m_bBorderVisible = FALSE;
	m_pRootView = NULL;
	m_pPrevFocusableView = NULL;
	m_pNextFocusableView = NULL;
	m_pAnimation = NULL;
	m_hCursor = NULL;
	m_pParent = parent;
	
	m_params.nRow = -1;
	m_params.nColumn = -1;
	m_params.nRowSpan = 1;
	m_params.nColumnSpan = 1;
	m_params.alignment.cx = UIAlignment::ALIGN_HCENTER;
	m_params.alignment.cy = UIAlignment::ALIGN_VCENTER;

	if(m_pParent)
		m_pParent->AddChildView(this);
}

UIView::~UIView()
{
	if(m_pParent)
		m_pParent->RemoveChildView(this);

	int x = static_cast<int>(m_childViews.size());
	while(--x >= 0)
	{
		m_childViews[x]->SetParent(NULL);
		delete m_childViews[x];
	}

	if(m_pAnimation)
		delete m_pAnimation;
}

void UIView::OnInit()
{
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* v = GetChildView(x);
		v->OnInit();
	}

	m_bInitialized = TRUE;
}

UIResponser* UIView::GetNextResponser() const
{
	return GetParent();
}

void UIView::SetBounds(const UIRect& rc)
{
	m_rcBounds.SetRect(rc.left, rc.top, rc.right, rc.bottom);
}

void UIView::SetBounds(int l, int t, int r, int b)
{
	m_rcBounds.SetRect(l, t, r, b);
}

const UIRect& UIView::GetBounds() const
{
	return m_rcBounds;
}

void UIView::SetPadding(int l, int t, int r, int b)
{
	m_rcPadding.SetRect(l, t, r, b);
}

void UIView::SetPadding(const UIRect& rc)
{
	m_rcPadding.SetRect(rc.left, rc.top, rc.right, rc.bottom);
}

const UIRect& UIView::GetPadding() const
{
	return m_rcPadding;
}

void UIView::SetMargin(int l, int t, int r, int b)
{
	m_rcMargin.SetRect(l, t, r, b);
}

void UIView::SetMargin(const UIRect& rc)
{
	SetMargin(rc.left, rc.top, rc.right, rc.bottom);
}

const UIRect& UIView::GetMargin() const
{
	return m_rcMargin;
}

void UIView::SetSize(const UISize& sz)
{
	SetSize(sz.cx, sz.cy);
}

void UIView::SetSize(int cx, int cy)
{
	int right = m_rcBounds.left + cx;
	int bottom = m_rcBounds.top + cy;
	SetBounds(m_rcBounds.left, m_rcBounds.top, right, bottom);
}

UISize UIView::GetSize() const
{
	return UISize(m_rcBounds.Width(), m_rcBounds.Height());
}

void UIView::SetPosition(const UIPoint& pt)
{
	SetPosition(pt.x, pt.y);
}

void UIView::SetPosition(int x, int y)
{
	int nWidth = m_rcBounds.Width();
	int nHeight = m_rcBounds.Height();
	m_rcBounds.SetRect(x, y, x + nWidth, y + nHeight);
}

UIPoint UIView::GetPosition() const
{
	return UIPoint(m_rcBounds.left, m_rcBounds.top);
}

void UIView::SetVisible(BOOL b)
{
	if(m_bVisible == b)
		return;

	m_bVisible = b;
	Invalidate();
}

BOOL UIView::IsVisible() const
{
	return m_bVisible;
}

BOOL UIView::IsFocusFrameVisible() const
{
	return TRUE;
}

void UIView::SetEnabled(BOOL b)
{
	m_bEnabled = b;
	Invalidate();
}

BOOL UIView::IsEnabled() const
{
	return m_bEnabled;
}

void UIView::SetFocusable(BOOL b)
{
	m_bFocusable = b;
	Invalidate();
}

BOOL UIView::IsFocusable() const
{
	return m_bFocusable && IsEnabled() && IsVisible();
}

BOOL UIView::IsInitialized() const
{
	return m_bInitialized;
}

void UIView::SetChecked(BOOL b)
{
	m_bChecked = b;
	Invalidate();
}

BOOL UIView::IsChecked() const
{
	return m_bChecked && IsEnabled();
}

void UIView::Activate(BOOL b)
{
	m_bActivated = b;
}

BOOL UIView::IsActivated() const
{
	return m_bActivated;
}

void UIView::SetBorderVisible(BOOL b)
{
	m_bBorderVisible = b;
}

BOOL UIView::IsBorderVisible() const
{
	return m_bBorderVisible;
}

void UIView::SetBorderColor(COLORREF clrFrame)
{
	m_clrFrame = clrFrame;
}

COLORREF UIView::GetBorderColor() const
{
	return m_clrFrame;
}

BOOL UIView::HasFocus()
{
	UIRootView* pRoot = GetRootView();
	XASSERT(pRoot);
	return pRoot->GetFocusedView() == this ? TRUE : FALSE;
}

INT_PTR UIView::GetTag() const
{
	return m_nTag;
}

void UIView::SetTag(INT_PTR nTag)
{
	m_nTag = nTag;
}

void UIView::SetID(const UIString& str)
{
	m_szID = str;
}

UIString UIView::GetID() const
{
	return m_szID;
}

void UIView::SetGroup(const UIString& str)
{
	m_szGroup = str;
}

UIString UIView::GetGroup() const
{
	return m_szGroup;
}

void UIView::SetSizePolicy(UISizePolicy::Policy horz, UISizePolicy::Policy vert)
{
	m_szPolicy.cx = horz;
	m_szPolicy.cy = vert;
}

const UISizePolicy& UIView::GetSizePolicy() const
{
	return m_szPolicy;
}

void UIView::SetBkgnd(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_bkgnd.SetBitmap(pBitmap);
	m_bkgnd.SetPos(rcPos);
}

void UIView::SetSkin(const UIString& szSkinXmlFile)
{
	m_szSkinXmlFile = szSkinXmlFile;
	const tinyxml2::XMLDocument* pDoc = UIApplication::Instance()->GetAssetManager()->GetSkin(m_szSkinXmlFile);
	if(pDoc)
	{
		const tinyxml2::XMLElement* pRoot = pDoc->RootElement();
		const tinyxml2::XMLAttribute* pAttr = pRoot->FirstAttribute();
		while(pAttr)
		{
			UIString szAttrName = UIAsset::UTF8ToString(pAttr->Name());
			UIString szAttrValue = UIAsset::UTF8ToString(pAttr->Value());
			SetAttribute(szAttrName, szAttrValue);
			pAttr = pAttr->Next();
		}
	}
}

UIString UIView::GetSkin() const
{
	return m_szSkinXmlFile;
}

UINT UIView::StartTimer(UINT_PTR uTimerID, int nInterval)
{
	UIRootView* pRootView = GetRootView();
	XASSERT(pRootView);
	return pRootView ? pRootView->SetTimer(this, uTimerID, nInterval) : NULL;
}

void UIView::KillTimer(UINT_PTR nTimerID)
{
	UIRootView* pRoot = GetRootView();
	XASSERT(pRoot);
	if(pRoot)
		pRoot->KillTimer(nTimerID);
}

BOOL UIView::HitTest(const UIPoint& pt) const
{
	UIRect rcTest(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	if(!m_rcClip.IsRectEmpty())
		rcTest.IntersectRect(rcTest, m_rcClip);

	return rcTest.PtInRect(pt);
}

BOOL UIView::HitTest(int x, int y) const
{
	UIPoint pt(x, y);
	UIRect rcTest(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	if(!m_rcClip.IsRectEmpty())
		rcTest.IntersectRect(rcTest, m_rcClip);

	return rcTest.PtInRect(pt);
}

void UIView::SetCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
}

HCURSOR UIView::GetCursor() const
{
	return m_hCursor;
}

void UIView::SetTooltipText(const UIString& szText)
{
	m_szTooltip = szText;
}

UIString UIView::GetTooltipText() const
{
	return m_szTooltip;
}

void UIView::AddChildView(UIView* v)
{
	AddChildView(v, static_cast<int>(m_childViews.size()));
}

void UIView::AddChildView(UIView* v, int nIndex)
{
	XASSERT(v != this);

	if(v->GetParent())
		v->GetParent()->RemoveChildView(v);

	v->SetParent(this);
	m_childViews.insert(m_childViews.begin() + nIndex, v);

	if(v->GetRootView())
	{
		v->OnInit();
	}
}

UIView* UIView::RemoveChildView(int nIndex)
{
	UIView* v = GetChildView(nIndex);
	return v ? RemoveChildView(v) : NULL;
}

UIView* UIView::RemoveChildView(UIView* v)
{	
	XASSERT(v != this);
	ViewList::iterator it = std::find(m_childViews.begin(), m_childViews.end(), v);
	if(it != m_childViews.end())
	{
		UIView* pNextFocusable = v->m_pNextFocusableView;
		UIView* pPrevFocusable = v->m_pPrevFocusableView;
		if(pPrevFocusable)
			pPrevFocusable->m_pNextFocusableView = pNextFocusable;
		if(pNextFocusable)
			pNextFocusable->m_pPrevFocusableView = pPrevFocusable;

		m_childViews.erase(it);

		v->SetParent(NULL);
		return v;
	}
	else
	{
		return NULL;
	}
}

void UIView::RemoveAllChildView()
{
	ViewList::iterator it;
	while((it = m_childViews.begin()) != m_childViews.end())
	{
		UIView* v = RemoveChildView(*it);
	}
}

int UIView::GetChildViewCount() const
{
	return m_childViews.size();
}

UIView* UIView::GetChildView(int nIndex) const
{
	return nIndex < GetChildViewCount() ? m_childViews[nIndex] : NULL;
}

void UIView::SetExtendedLayoutParams(const ExtendedLayoutParams& props)
{
	m_params = props;
}

const ExtendedLayoutParams& UIView::GetExtendedLayoutParams() const
{
	return m_params;
}

void UIView::SetAnimation(UIAnimation* pAnimation)
{
	if(m_pAnimation)
	{
		m_pAnimation->Stop();
		delete m_pAnimation;
	}

	m_pAnimation = pAnimation;
}

UIAnimation* UIView::GetAnimation()
{
	return m_pAnimation;
}

void UIView::SetParent(UIView* v)
{
	m_pParent = v;
}

UIView* UIView::GetParent() const
{
	return m_pParent;
}

void UIView::SetClipRect(const UIRect& rcClip)
{
	m_rcClip = rcClip;
}

void UIView::SetClipRect(int l, int t, int r, int b)
{
	m_rcClip.SetRect(l, t, r, b);
}

const UIRect& UIView::GetClipRect() const
{
	return m_rcClip;
}

BOOL UIView::IsParentOf(UIView* v) const
{
	XASSERT(v);
	UIView* pParent = v->GetParent();
	while(pParent)
	{
		if(this == pParent)
			return TRUE;

		pParent = pParent->GetParent();
	}

	return FALSE;
}

UIView* UIView::GetViewForID(const UIString& id)
{
	if(m_szID == id)
		return this;

	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* pChild = GetChildView(x);
		UIView* pView = pChild->GetViewForID(id);
		if(pView)
			return pView;
	}

	return NULL;
}

UIView* UIView::GetViewForPoint(const UIPoint& pt)
{
	if(!HitTest(pt))
		return NULL;

	for(int x = GetChildViewCount() - 1; x >= 0; --x)
	{
		UIView* pChild = GetChildView(x);
		if(!pChild->IsVisible() || !pChild->IsEnabled())
			continue;

		UIPoint pos = pChild->GetPosition();
		pos.x = -pos.x + pt.x;
		pos.y = -pos.y + pt.y;
		if(pChild->HitTest(pos))
		{
			return pChild->GetViewForPoint(pos);
		}
	}

	return this;
}

UIView* UIView::GetSelectedViewForGroup(const UIString& szGroup)
{
	ViewList views;
	GetViewsWithGroup(szGroup, &views);
	ViewList::iterator itor;
	for(itor = views.begin(); itor != views.end(); ++itor)
	{
		if((*itor)->IsChecked())
			return *itor;
	}

	return NULL;
}

void UIView::GetViewsWithGroup(const UIString& szGroup, std::vector<UIView*>* pOut)
{
	if(m_szGroup == szGroup)
	{
		pOut->push_back(this);
	}

	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		GetChildView(x)->GetViewsWithGroup(szGroup, pOut);
	}
}

void UIView::GetViewsWithTabstop(std::vector<UIView*>* pOut)
{
	if(m_nTabIndex != 0)
	{
		pOut->push_back(this);
	}

	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		GetChildView(x)->GetViewsWithTabstop(pOut);
	}
}

UIView* UIView::GetNextFocusableView()
{
	return m_pNextFocusableView;
}

UIView* UIView::GetPrevFocusableView()
{
	return m_pPrevFocusableView;
}

void UIView::SetNextFocusableView(UIView* v)
{
	UIView* pNextFocusable = m_pNextFocusableView;
	if(pNextFocusable)
	{
		pNextFocusable->m_pPrevFocusableView = v;
		v->m_pNextFocusableView = pNextFocusable;
	}

	m_pNextFocusableView = v;
	v->m_pPrevFocusableView = this;
}

void UIView::SetPrevFocusableView(UIView* v)
{
	UIView* pPrevFocusable = m_pPrevFocusableView;
	if(pPrevFocusable)
	{
		pPrevFocusable->m_pNextFocusableView = v;
		v->m_pPrevFocusableView = pPrevFocusable;
	}

	m_pPrevFocusableView = v;
	v->m_pNextFocusableView = this;
}

UIPoint UIView::MapToWindow(UIView* pView, const UIPoint& pt)
{
	UIPoint point(0, 0);
	point.Offset(pt.x, pt.y);
	const UIView* pParent = pView;
	UIPoint pos;
	while(pParent)
	{
		pos = pParent->GetPosition();
		point.Offset(pos.x, pos.y);
		pParent = pParent->GetParent();
	}

	return point;
}

UIPoint UIView::MapToScreen(UIView* pView, const UIPoint& pt)
{
	HWND hWnd = pView->GetRootView()->GetWindow()->GetNativeWindow();
	UIPoint point = MapToWindow(pView, pt);
	::ClientToScreen(hWnd, point);
	return point;
}

UIRect UIView::MapToScreen(UIView* pView, const UIRect& rc)
{
	UIPoint p1(rc.left, rc.top);
	UIPoint p2 = MapToScreen(pView, p1);
	return UIRect(p2.x, p2.y, p2.x + rc.Width(), p2.y + rc.Height());
}

UIRect UIView::MapToWindow(UIView* pView, const UIRect& rc)
{
	UIPoint p1(rc.left, rc.top);
	UIPoint p2 = MapToWindow(pView, p1);
	return UIRect(p2.x, p2.y, p2.x + rc.Width(), p2.y + rc.Height());
}

UIPoint UIView::MapToView(UIView* pView, const UIPoint& pt)
{
	UIPoint point(0, 0);
	UIPoint ptGlobal = MapToWindow(pView, point);
	return UIPoint(pt.x - ptGlobal.x, pt.y - ptGlobal.y);
}

UIRect UIView::MapToView(UIView* pView, const UIRect& rc)
{
	UIPoint p1(rc.left, rc.top);
	UIPoint p2 = MapToView(pView, p1);
	return UIRect(p2.x, p2.y, p2.x + rc.Width(), p2.y + rc.Height());
}

UIRootView* UIView::GetRootView()
{
	if(m_pRootView)
		return m_pRootView;

	if(m_pParent)
		m_pRootView = m_pParent->GetRootView();

	return m_pRootView;
}

void UIView::RequestLayout()
{
	m_bNeedLayout = TRUE;
	if(m_pParent)
	{
		m_pParent->RequestLayout();
	}
	else
	{
		OnLayout();
	}
}

void UIView::Invalidate()
{
	UIRootView* pRootView = GetRootView();
	if(pRootView)
	{
		UIRect rcTemp(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
		UIRect rcDirty = MapToWindow(this, rcTemp);
		::InvalidateRect(pRootView->GetWindow()->GetNativeWindow(), rcDirty, FALSE);
	}
}

void UIView::DoPaint(HDC hdc, const UIRect& rcDirty)
{
	if(!IsVisible())
		return;

	UIRect rcTemp(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	if(!m_rcClip.IsRectEmpty())
		rcTemp.IntersectRect(rcTemp, m_rcClip);

	UIPoint rcPos = MapToWindow(this, UIPoint(0, 0));
	UIRect rcView(rcPos.x + rcTemp.left, rcPos.y + rcTemp.top,
		rcPos.x + rcTemp.left + rcTemp.Width(), rcPos.y + rcTemp.top + rcTemp.Height());
	if(!rcView.IntersectRect(rcView, rcDirty))
		return;

	int nSave = ::SaveDC(hdc);
	HRGN hRgn = ::CreateRectRgn(rcView.left, rcView.top, rcView.right, rcView.bottom);
	::SelectClipRgn(hdc, hRgn);
	::SetViewportOrgEx(hdc, rcPos.x, rcPos.y, NULL);

	OnPaint(hdc);

	if(HasFocus() && IsFocusFrameVisible())
	{
		UIRect rcFocus(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
		rcFocus.DeflateRect(1, 1);
		UIRender::DrawFocusRect(hdc, rcFocus);
	}
	if(IsBorderVisible())
	{
		UIRect rc(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
		UIRender::DrawRect(hdc, rc, *UIApplication::Instance()->GetAssetManager()->GetBrush(m_clrFrame));
	}
	if(g_bOverlayVisible)
	{
		UIRect rc(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
		UIRender::DrawRect(hdc, rc, *UIApplication::Instance()->GetAssetManager()->GetBrush(RGB(255, 0, 0)));
	}
	::DeleteObject(hRgn);
	::RestoreDC(hdc, nSave);


	// paint children.
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* pChild = GetChildView(x);
		if(!pChild)
			continue;

		pChild->DoPaint(hdc, rcView);
	}
}

UIView* UIView::InflateFromXML(const tinyxml2::XMLElement* pNode)
{
	UIString szNodeName = UIAsset::UTF8ToString(pNode->Name());
	UIView* v = XML_NODE_INFO::CreateNode(szNodeName);
	XASSERT(v);
	const tinyxml2::XMLAttribute* pAttr = pNode->FirstAttribute();
	while(pAttr)
	{
		UIString szAttrName = UIAsset::UTF8ToString(pAttr->Name());
		UIString szAttrValue = UIAsset::UTF8ToString(pAttr->Value());
		v->SetAttribute(szAttrName, szAttrValue);
		pAttr = pAttr->Next();
	}

	const tinyxml2::XMLNode* pChild = pNode->FirstChild();
	while(pChild != NULL)
	{
		if(!pChild->ToComment())
		{
			UIView* pSubView = InflateFromXML(pChild->ToElement());
			v->AddChildView(pSubView);
		}
		pChild = pChild->NextSibling();
	}

	return v;
}

UIView* UIView::Inflate(const UIString& szXmlFileName, UIView* pAttachTo)
{
	const tinyxml2::XMLDocument* pDoc = UIApplication::Instance()->GetAssetManager()->GetLayout(szXmlFileName);
	XASSERT(pDoc);
	UIView* v = InflateFromXML(pDoc->RootElement());
	if(pAttachTo)
		pAttachTo->AddChildView(v);


	return v;
}

void UIView::InvalidateLayout()
{
	if(IsVisible())
	{
		OnLayout();
		for(int x = 0; x < GetChildViewCount(); ++x)
		{
			UIView* v = GetChildView(x);
			if(v)
				v->InvalidateLayout();
		}
	}
}

UISize UIView::GetPreferredSize() const
{
	UISize size = GetMinimumSize();
	if(m_bkgnd.IsValid())
	{
		size.cx = max(size.cx, m_bkgnd.Width());
		size.cy = max(size.cy, m_bkgnd.Height());
	}

	return size;
}

UISize UIView::GetMinimumSize() const
{
	return UISize(1, 1);
}

UISize UIView::GetMaximumSize() const
{
	return UISize(9999, 9999);
}

void UIView::SetOverlayVisible(BOOL b)
{
	if(g_bOverlayVisible == b)
		return;

	g_bOverlayVisible = b;
	Invalidate();
}

void UIView::SetViewportOrg(int x, int y)
{
	m_ptViewportOrg.x = x;
	m_ptViewportOrg.y = y;
	Invalidate();
}

const UIPoint& UIView::GetViewportOrg() const
{
	return m_ptViewportOrg;
}

void UIView::RequestFocus()
{
	UIRootView* pRoot = GetRootView();
	XASSERT(pRoot);
	if(pRoot)
		pRoot->SetFocusView(this);
}

void UIView::ClearFocus()
{
	UIRootView* pRoot = GetRootView();
	XASSERT(pRoot);
	if(HasFocus())
		pRoot->SetFocusView(NULL);
}

void UIView::SetOpacity(int nOpacity)
{
	m_nOpacity = nOpacity;
	Invalidate();
}

int UIView::GetOpacity() const
{
	return m_nOpacity;
}

void UIView::SetTabIndex(int nIndex)
{
	m_nTabIndex = nIndex;
}

int UIView::GetTabIndex() const
{
	return m_nTabIndex;
}
void UIView::OnLayout()
{
	m_bNeedLayout = FALSE;

	// layout children.
	for(int x = 0; x < GetChildViewCount(); ++x)
	{
		UIView* pChild = GetChildView(x);
		if(pChild->m_bNeedLayout)
		{
			pChild->m_bNeedLayout = FALSE;
			pChild->OnLayout();
		}
	}
}

void UIView::OnPaint(HDC hdc)
{

}

BOOL UIView::Event(UIEvent* event)
{
	switch(event->GetType())
	{
	case UIEvent::EVENT_MOUSE_PRESSED:
	{
		return OnMousePressed(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_RELEASED:
	{
		return OnMouseReleased(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_DRAGGED:
	{
		return OnMouseDragged(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_ENTERED:
	{
		return OnMouseEntered(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_EXITED:
	{
		return OnMouseExited(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_MOVED:
	{
		return OnMouseMoved(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_WHEEL:
	{
		return OnMouseWheel(static_cast<UIWheelEvent*>(event));
	}
	case UIEvent::EVENT_KEY_PRESSED:
	{
		UIKeyEvent* e = static_cast<UIKeyEvent*>(event);
		return OnKeyPressed(e);
	}
	case UIEvent::EVENT_KEY_RELEASED:
	{
		return OnKeyReleased(static_cast<UIKeyEvent*>(event));
	}
	case UIEvent::EVENT_FOCUS_GAIN:
	{
		return OnFocusGain(static_cast<UIFocusEvent*>(event));
	}
	case UIEvent::EVENT_FOCUS_LOST:
	{
		return OnFocusLost(static_cast<UIFocusEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_CLICKED:
	{
		return OnMouseClicked(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_MOUSE_DBLCLICKED:
	{
		return OnMouseDblClicked(static_cast<UIMouseEvent*>(event));
	}
	case UIEvent::EVENT_TIMER:
	{
		return OnTimer(static_cast<UITimerEvent*>(event));
	}
	case UIEvent::EVENT_ACTION:
	{
		return OnActionPerformed(static_cast<UIActionEvent*>(event));
	}
	}

	return FALSE;
}

void UIView::SetAttribute(const UIString& szAttrName, const UIString& szAttrValue)
{
	if(szAttrName == _T("id"))
	{
		m_szID = szAttrValue;
	}
	else if(szAttrName == _T("group"))
	{
		m_szGroup = szAttrValue;
	}
	else if(szAttrName == _T("tooltip"))
	{
		m_szTooltip = szAttrValue;
	}
	else if(szAttrName == _T("visible"))
	{
		if(szAttrValue == _T("true"))
			m_bVisible = TRUE;
		else
			m_bVisible = FALSE;
	}
	else if(szAttrName == _T("enabled"))
	{
		if(szAttrValue == _T("true"))
			m_bEnabled = TRUE;
		else
			m_bEnabled = FALSE;
	}
	else if(szAttrName == _T("focusable"))
	{
		if(szAttrValue == _T("true"))
			m_bFocusable = TRUE;
		else
			m_bFocusable = FALSE;
	}
	else if(szAttrName == _T("checked"))
	{
		if(szAttrValue == _T("true"))
			m_bChecked = TRUE;
		else
			m_bChecked = FALSE;
	}
	else if(szAttrName ==_T("tabindex"))
	{
		m_nTabIndex = _ttoi(szAttrValue);
	}
	else if(szAttrName == _T("width"))
	{
		if(szAttrValue == _T("preferred"))
		{
			m_szPolicy.cx = UISizePolicy::PREFERRED;
		}
		else if(szAttrValue == _T("expanding"))
		{
			m_szPolicy.cx = UISizePolicy::EXPANDING;
		}
		else
		{
			m_szPolicy.cx = UISizePolicy::FIXED;
			m_rcBounds.right = m_rcBounds.left + _ttoi(szAttrValue);
		}
	}
	else if(szAttrName == _T("height"))
	{
		if(szAttrValue == _T("preferred"))
		{
			m_szPolicy.cy = UISizePolicy::PREFERRED;
		}
		else if(szAttrValue == _T("expanding"))
		{
			m_szPolicy.cy = UISizePolicy::EXPANDING;
		}
		else
		{
			m_szPolicy.cy = UISizePolicy::FIXED;
			m_rcBounds.bottom = m_rcBounds.top + _ttoi(szAttrValue);
		}
	}
	else if(szAttrName == _T("margin"))
	{
		int nPos = 0;
		UIString szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcMargin.left = _ttoi(szTemp);
		szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcMargin.top = _ttoi(szTemp);
		szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcMargin.right = _ttoi(szTemp);
		szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcMargin.bottom = _ttoi(szTemp);
	}
	else if(szAttrName == _T("padding"))
	{
		int nPos = 0;
		UIString szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcPadding.left = _ttoi(szTemp);
		szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcPadding.top = _ttoi(szTemp);
		szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcPadding.right = _ttoi(szTemp);
		szTemp = szAttrValue.Tokenize(_T(","), nPos);
		if(!szTemp.IsEmpty())
			m_rcPadding.bottom = _ttoi(szTemp);
	}
	else if(szAttrName == _T("bkImage"))
	{
		UIString szValue = szAttrValue;
		if(szValue.StartWidth(_T("#")))
		{
	
			int nLen = szValue.GetLength();
			if(szValue.GetLength() >= sizeof(_T("#FFFFFF")) / sizeof(TCHAR) - 1)
			{
				int nPos = szValue.Find(_T("#"));
				if(nPos != -1)
				{
					szValue.MakeUpper();
					int r = 0, g = 0, b = 0, a = 0;
					BOOL bTrans = FALSE;
					if(nLen >= 9)
					{
						bTrans = TRUE;
						_stscanf(szValue.Mid(nPos + 1, 2), _T("%X"), &a);
						_stscanf(szValue.Mid(nPos + 3, 2), _T("%X"), &r);
						_stscanf(szValue.Mid(nPos + 5, 2), _T("%X"), &g);
						_stscanf(szValue.Mid(nPos + 7, 2), _T("%X"), &b);
					}
					else
					{
						_stscanf(szValue.Mid(nPos + 1, 2), _T("%X"), &r);
						_stscanf(szValue.Mid(nPos + 3, 2), _T("%X"), &g);
						_stscanf(szValue.Mid(nPos + 5, 2), _T("%X"), &b);
					}
					const UIBitmap* pBkBitmap = UIApplication::Instance()->GetAssetManager()->GetRGBImage(RGB(r, g, b), bTrans, a); \
					if(pBkBitmap)
						m_bkgnd.SetBitmap(pBkBitmap);
				}
			}
		}
		else
		{
			int nStart = szAttrValue.Find(_T("source("));
			int nEnd = szAttrValue.ReverseFind(_T(')'));
			UIString szImageName;
			if(nStart != -1)
				szImageName = szAttrValue.Left(nStart);
			else
				szImageName = szAttrValue;
			szImageName.Trim();
			const UIBitmap* pBkBitmap = UIApplication::Instance()->GetAssetManager()->GetImage(szImageName);
			if(!pBkBitmap)
				return;

			m_bkgnd.SetBitmap(pBkBitmap);
			if(nStart != -1 && nEnd != -1)
			{
				int nIndex = nStart + sizeof(_T("source(")) / sizeof(TCHAR) - 1;
				UIString szRect = szAttrValue.Mid(nIndex, nEnd - nIndex);
				szRect.Trim();
				UIRect rcPos;

				int nPos = 0;
				UIString szTemp = szRect.Tokenize(_T(","), nPos);
				if(!szTemp.IsEmpty())
					rcPos.left = _ttoi(szTemp);
				szTemp = szRect.Tokenize(_T(","), nPos);
				if(!szTemp.IsEmpty())
					rcPos.top = _ttoi(szTemp);
				szTemp = szRect.Tokenize(_T(","), nPos);
				if(!szTemp.IsEmpty())
					rcPos.right = _ttoi(szTemp);
				szTemp = szRect.Tokenize(_T(","), nPos);
				if(!szTemp.IsEmpty())
					rcPos.bottom = _ttoi(szTemp);

				m_bkgnd.SetPos(rcPos);
			}
		}
	}
	else if(szAttrName == _T("skin"))
	{
		SetSkin(szAttrValue);
	}
	else if(szAttrName == _T("opacity"))
	{
		m_nOpacity = _ttoi(szAttrValue);
	}
	else if(szAttrName == _T("layoutRow"))
	{
		m_params.nRow = _ttoi(szAttrValue);
	}
	else if(szAttrName == _T("layoutColumn"))
	{
		m_params.nColumn = _ttoi(szAttrValue);
	}
	else if(szAttrName == _T("layoutRowSpan"))
	{
		m_params.nRowSpan = _ttoi(szAttrValue);
	}
	else if(szAttrName == _T("layoutColumnSpan"))
	{
		m_params.nColumnSpan = _ttoi(szAttrValue);
	}
	else if(szAttrName == _T("layoutGravity"))
	{
		int nPos = 0;
		UIString szTemp = szAttrValue.Tokenize(_T("|"), nPos);
		if(!szTemp.IsEmpty())
		{
			szTemp.MakeUpper();
			if(szTemp == _T("LEFT"))
				m_params.alignment.cx = UIAlignment::ALIGN_LEFT;
			else if(szTemp == _T("HCENTER"))
				m_params.alignment.cx = UIAlignment::ALIGN_HCENTER;
			else if(szTemp == _T("RIGHT"))
				m_params.alignment.cx = UIAlignment::ALIGN_RIGHT;

			szTemp = szAttrValue.Tokenize(_T(","), nPos);
			if(!szTemp.IsEmpty())
			{
				szTemp.MakeUpper();
				if(szTemp == _T("TOP"))
					m_params.alignment.cy = UIAlignment::ALIGN_TOP;
				else if(szTemp == _T("VCENTER"))
					m_params.alignment.cy = UIAlignment::ALIGN_VCENTER;
				else if(szTemp == _T("BOTTOM"))
					m_params.alignment.cy = UIAlignment::ALIGN_BOTTOM;
			}
		}
		else
		{
			if(!szTemp.IsEmpty())
			{
				szTemp.MakeUpper();
				if(szTemp == _T("LEFT"))
					m_params.alignment.cx = UIAlignment::ALIGN_LEFT;
				else if(szTemp == _T("HCENTER"))
					m_params.alignment.cx = UIAlignment::ALIGN_HCENTER;
				else if(szTemp == _T("RIGHT"))
					m_params.alignment.cx = UIAlignment::ALIGN_RIGHT;
			}
		}
	}
	else if(szAttrName == _T("borderColor"))
	{
		UIString szValue = szAttrValue;
		if(szValue.StartWidth(_T("#")))
		{
			if(szValue.GetLength() >= (sizeof(_T("#FFFFFF")) / sizeof(TCHAR) - 1))
			{
				int nPos = szValue.Find(_T("#"));
				if(nPos != -1)
				{
					szValue.MakeUpper();
					int r, g, b;
					_stscanf(szValue.Mid(nPos + 1, 2), _T("%X"), &r);
					_stscanf(szValue.Mid(nPos + 3, 2), _T("%X"), &g);
					_stscanf(szValue.Mid(nPos + 5, 2), _T("%X"), &b);
					m_clrFrame = RGB(r, g, b);
				}
			}
		}
	}
	else if(szAttrName == _T("borderVisible"))
	{
		if(szAttrValue == _T("true"))
			m_bBorderVisible = TRUE;
		else
			m_bBorderVisible = FALSE;
	}
	else
	{
		if (!OnCustomAttribute(szAttrName, szAttrValue))
		{
			TCHAR szMsg[256] = { 0 };
			_sntprintf(szMsg, sizeof(szMsg) / sizeof(TCHAR),
				_T("UNSUPPORTED XML PROPERTY [%s=%s] OF %s"), szAttrName, szAttrValue, GetNodeName());
			::MessageBox(NULL, szMsg, _T("WARNING"), MB_OK);
		}
	}
}

BOOL UIView::OnCustomAttribute(const UIString& szAttrName, const UIString& szAttrValue)
{
	return FALSE;
}

BOOL UIView::OnMouseDblClicked(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMousePressed(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMouseDragged(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMouseReleased(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMouseMoved(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMouseEntered(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMouseExited(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMouseWheel(UIWheelEvent* event)
{
	return TRUE;
}

BOOL UIView::OnKeyPressed(UIKeyEvent* event)
{
	return TRUE;
}

BOOL UIView::OnKeyReleased(UIKeyEvent* event)
{
	return TRUE;
}

BOOL UIView::OnFocusGain(UIFocusEvent* event)
{
	return TRUE;
}

BOOL UIView::OnFocusLost(UIFocusEvent* event)
{
	return TRUE;
}

BOOL UIView::OnMouseClicked(UIMouseEvent* event)
{
	return TRUE;
}

BOOL UIView::OnTimer(UITimerEvent* event)
{
	return FALSE;
}

BOOL UIView::OnActionPerformed(UIActionEvent* event)
{
	return FALSE;
}