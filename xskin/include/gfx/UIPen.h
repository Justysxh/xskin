#ifndef __UIPEN_H_INCLUDED__
#define __UIPEN_H_INCLUDED__

#include <map>
#include "../core/RefCounted.h"
#include "../core/UIObject.h"
#include "../core/UIString.h"

class XSKIN_API UIPen : public UIObject, public RefCounted
{
public:
	static UIPen*	GetPen(LPCTSTR szCacheKey);
	static UIPen*	CreateNew(HPEN hPen, LPCTSTR szCacheKey = NULL);
	static UIPen*	CreateNew(COLORREF clr, int nWidth, LPCTSTR szCacheKey = NULL);
	HPEN			GetNativePen() const;
	operator		HPEN() const;

protected:

					UIPen(HPEN hPen);
					UIPen(COLORREF clr, int nWidth);
					~UIPen();
	void			Init();

private:
	HPEN m_hPen;
	UIString m_szKey;
	typedef std::map<UIString, UIPen*> GdiPenCache;
	static GdiPenCache cache;
};
#endif //! __UIPEN_H_INCLUDED__