#ifndef __UIRENDER_H_INCLUDED__
#define __UIRENDER_H_INCLUDED__

#include "../core/UIObject.h"
#include "../core/UIString.h"
#include "../core/UIPoint.h"
#include "../core/UISize.h"
#include "../core/UIRect.h"
#include "../core/UIMeasure.h"
#include "../gfx/UIBitmap.h"
#include "../gfx/UIPen.h"
#include "../gfx/UIBrush.h"

class XSKIN_API UIRender : public UIObject
{
public:
	UIRender();
	~UIRender();
	static void MeasureString(HDC hdc, UIRect& rcOut, const UIString& szText, const UIAlignment& align, BOOL bMultiline = FALSE);
	static void DrawString(HDC hdc, const UIRect& rc, const UIString& szText, const UIAlignment& align, BOOL bMultiline = FALSE);
	static void DrawLine(HDC hdc, const UIPoint& p1, const UIPoint& p2);
	static void DrawRect(HDC hdc, const UIRect& rc, const UIBrush& hbr);
	static void DrawFocusRect(HDC hdc, const UIRect& rc);
	static void FillRect(HDC hdc, const UIRect& rc, const UIBrush& hbr);
	static void DrawBitmap(HDC hdc, const UIRect& dst, const UIRect& src, const UIBitmap* pBitmap, int nOpacity = 255);
	static void DrawBitmap(HDC hdc, const UIRect& dst, const UIBitmap* pBitmap, int nOpacity = 255);
};
#endif //! __UIRENDER_H_INCLUDED__