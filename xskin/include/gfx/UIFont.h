#ifndef __UIFONT_H_INCLUDED__
#define __UIFONT_H_INCLUDED__

#include <map>
#include "../core/UIObject.h"
#include "../core/RefCounted.h"
#include "../core/UIString.h"

class XSKIN_API UIFont : public UIObject, public RefCounted
{
public:
	enum FontStyle
	{
		NORMAL		= 1 << 0,
		BOLD		= 1 << 1,
		ITALIC		= 1 << 2,
		UNDERLINED	= 1 << 3
	};

public:
	static UIFont*	GetFont(LPCTSTR szCacheKey);
	static UIFont*	CreateNew(HFONT hFont, LPCTSTR szCacheKey = NULL);
	static UIFont*	CreateNew(const UIString& szFontName, int nFontSize, int nFontStyle, LPCTSTR szCacheKey = NULL);

	int				GetHeight() const;
	int				GetBaseline() const;
	int				GetAverageCharWidth() const;
	int				GetStringWidth(const UIString& szText) const;
	int				GetFontStyle() const;
	int				GetFontSize() const;
	UIString		GetFontName() const;
	HFONT			GetNativeFont() const;
	operator		HFONT() const;

protected:
					UIFont(HFONT hFont);
					UIFont(const UIString& szFontName, int nFontSize, int nFontStyle);
					~UIFont();
	void			Init();

private:
	HFONT m_hFont;
	UIString m_szKey;
	UIString m_szName;
	int m_nStyle;
	int m_nSize;
	int m_nHeight;
	int m_nBaseline;
	int m_nAvgCharWidth;

	typedef std::map<UIString, UIFont*> FontCache;
	typedef std::map<UIString, int> StringWidthCache;
	static FontCache fontCache;
};

#endif //! __UIFONT_H_INCLUDED__