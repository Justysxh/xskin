#ifndef __UIBITMAP_H_INCLUDED__
#define __UIBITMAP_H_INCLUDED__

#include <map>
#include "../core/UIObject.h"
#include "../core/UIString.h"
#include "../core/RefCounted.h"

class XSKIN_API UIBitmap : public UIObject, public RefCounted
{
public:
	static UIBitmap*	GetBitmap(LPCTSTR szCacheKey);
	static UIBitmap*	CreateNew(HBITMAP hBitmap, LPCTSTR szCacheKey = NULL);
	static UIBitmap*	CreateNew(int w, int h, int nBitCount, LPCVOID lpvBits, LPCTSTR szCacheKey = NULL);

	BOOL				HasAlpha() const;
	int					GetWidth() const;
	int					GetHeight() const;
	HBITMAP				GetNativeBitmap() const;
	operator			HBITMAP() const;

protected:
						UIBitmap(HBITMAP hBitmap);
						UIBitmap(int w, int h, int nBitCount, LPCVOID lpvBits);
						~UIBitmap();
	void				Init();

private:
	UIString m_szKey;
	HBITMAP m_hBitmap;
	int m_nWidth;
	int m_nHeight;
	BOOL m_bAlphaChannel;

	typedef std::map<UIString, UIBitmap*> BitmapCache;
	static BitmapCache cache;
};
#endif //! __UIBITMAP_H_INCLUDED__