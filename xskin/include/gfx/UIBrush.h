#ifndef __UIBRUSH_H_INCLUDED__
#define __UIBRUSH_H_INCLUDED__

#include <map>
#include "../core/RefCounted.h"
#include "../core/UIObject.h"
#include "../core/UIString.h"

class XSKIN_API UIBrush : public UIObject, public RefCounted
{
public:
	static UIBrush*	GetBrush(LPCTSTR szCacheKey);
	static UIBrush*	CreateNew(HBRUSH hBrush, LPCTSTR szCacheKey = NULL);
	static UIBrush*	CreateNew(COLORREF clr, LPCTSTR szCacheKey = NULL);
	HBRUSH			GetNativeBrush() const;
	operator		HBRUSH() const;

protected:

					UIBrush(HBRUSH hBrush);
					UIBrush(COLORREF clr);
					~UIBrush();
	void			Init();

private:
	HBRUSH m_hBrush;
	UIString m_szKey;
	typedef std::map<UIString, UIBrush*> GdiBrushCache;
	static GdiBrushCache cache;
};
#endif //! __UIBRUSH_H_INCLUDED__