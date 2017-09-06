#include "stdafx.h"
#include "../../include/gfx/UIFont.h"

UIFont::FontCache UIFont::fontCache;

UIFont::UIFont(HFONT hFont)
{
	m_hFont = hFont;
	Init();
}

UIFont::UIFont(const UIString& szFontName, int nFontSize, int nFontStyle)
{
	m_szName = szFontName;
	m_nSize = nFontSize;
	HDC hdc = GetDC(NULL);
	int nHeight = -MulDiv(nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(NULL, hdc);

	int nWeight = FW_NORMAL;
	BOOL bItalic = FALSE;
	BOOL bUnderlined = FALSE;

	if(nFontStyle & BOLD)
		nWeight = FW_BOLD;
	if(nFontStyle & ITALIC)
		bItalic = TRUE;
	if(nFontStyle & UNDERLINED)
		bUnderlined = TRUE;

	m_hFont = ::CreateFont(nHeight, 0, 0, 0, nWeight, bItalic, bUnderlined, 0, 0, 0, 0, 0, 0, szFontName);
	XASSERT(m_hFont);

	Init();
}

UIFont::~UIFont()
{
	FontCache::iterator itor = fontCache.find(m_szKey);
	if(itor != fontCache.end())
		fontCache.erase(itor);

	if(m_hFont)
		::DeleteObject(m_hFont);
}

UIFont* UIFont::GetFont(LPCTSTR szCacheKey)
{
	FontCache::const_iterator itor = fontCache.find(szCacheKey);
	if(itor != fontCache.end())
	{
		itor->second->AddRef();
		return itor->second;
	}
	else
	{
		return NULL;
	}
}

UIFont* UIFont::CreateNew(HFONT hFont, LPCTSTR szCacheKey)
{
	UIFont* pFont;
	if(szCacheKey)
	{
		FontCache::const_iterator itor = fontCache.find(szCacheKey);
		if(itor != fontCache.end())
		{
			pFont = itor->second;
			pFont->AddRef();
		}
		else
		{
			pFont = new UIFont(hFont);
			pFont->m_szKey = szCacheKey;
			fontCache.insert(std::make_pair(szCacheKey, pFont));
		}
	}
	else
	{
		pFont = new UIFont(hFont);
	}

	return pFont;
}

UIFont* UIFont::CreateNew(const UIString& szFontName, int nFontSize, int nFontStyle, LPCTSTR szCacheKey)
{
	UIFont* pFont;
	if(szCacheKey)
	{
		FontCache::const_iterator itor = fontCache.find(szCacheKey);
		if(itor != fontCache.end())
		{
			pFont = itor->second;
			pFont->AddRef();
		}
		else
		{
			pFont = new UIFont(szFontName, nFontSize, nFontStyle);
			pFont->m_szKey = szCacheKey;
			fontCache.insert(std::make_pair(szCacheKey, pFont));
		}
	}
	else
	{
		pFont = new UIFont(szFontName, nFontSize, nFontStyle);
	}

	return pFont;
}

void UIFont::Init()
{
	TEXTMETRIC metrics;
	HDC hScreenDC = GetDC(NULL);
	HFONT hOldFont = static_cast<HFONT>(SelectObject(hScreenDC, m_hFont));
	int nOldMapMode = SetMapMode(hScreenDC, MM_TEXT);
	GetTextMetrics(hScreenDC, &metrics);
	SelectObject(hScreenDC, hOldFont);
	SetMapMode(hScreenDC, nOldMapMode);
	ReleaseDC(NULL, hScreenDC);

	m_nHeight = max(1, static_cast<int>(metrics.tmHeight));
	m_nBaseline = max(1, static_cast<int>(metrics.tmAscent));
	m_nAvgCharWidth = max(1, static_cast<int>(metrics.tmAveCharWidth));
	m_nStyle = UIFont::NORMAL;
	if(metrics.tmItalic)
		m_nStyle |= UIFont::ITALIC;
	if(metrics.tmUnderlined)
		m_nStyle |= UIFont::UNDERLINED;
	if(metrics.tmWeight >= FW_BOLD)
		m_nStyle |= UIFont::BOLD;

	AddRef();
}

int UIFont::GetHeight() const
{
	return m_nHeight;
}

int UIFont::GetBaseline() const
{
	return m_nBaseline;
}

int UIFont::GetAverageCharWidth() const
{
	return m_nAvgCharWidth;
}

int UIFont::GetStringWidth(const UIString& szText) const
{
	HDC hdc = GetDC(NULL);
	HFONT hOldFont = static_cast<HFONT>(SelectObject(hdc, m_hFont));
	int nOldMap = SetMapMode(hdc, MM_TEXT);
	SIZE size;
	GetTextExtentPoint32(hdc, szText, szText.GetLength(), &size);
	SelectObject(hdc, hOldFont);
	SetMapMode(hdc, nOldMap);
	ReleaseDC(NULL, hdc);

	return size.cx;
}

int UIFont::GetFontStyle() const
{
	return m_nStyle;
}

int UIFont::GetFontSize() const
{
	return m_nSize;
}

UIString UIFont::GetFontName() const
{
	return m_szName;
}

HFONT UIFont::GetNativeFont() const
{
	return m_hFont;
}

UIFont::operator HFONT() const
{
	return m_hFont;
}