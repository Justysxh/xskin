#include "stdafx.h"
#include "../../include/gfx/UIBitmap.h"

UIBitmap::BitmapCache UIBitmap::cache;


UIBitmap::UIBitmap(HBITMAP hBitmap)
{
	m_hBitmap = hBitmap;
	Init();
}

UIBitmap::UIBitmap(int w, int h, int nBitCount, LPCVOID lpvBits)
{
	m_hBitmap = ::CreateBitmap(w, h, 1, nBitCount, lpvBits);
	Init();
}

UIBitmap::~UIBitmap()
{
	BitmapCache::iterator itor = cache.find(m_szKey);
	if(itor != cache.end())
		cache.erase(itor);

	if(m_hBitmap)
		::DeleteObject(m_hBitmap);
}

UIBitmap* UIBitmap::GetBitmap(LPCTSTR szCacheKey)
{
	BitmapCache::const_iterator itor = cache.find(szCacheKey);
	if(itor != cache.end())
	{
		itor->second->AddRef();
		return itor->second;
	}
	else
	{
		return NULL;
	}
}

void UIBitmap::Init()
{
	BITMAP bm;
	GetObject(m_hBitmap, sizeof(BITMAP), &bm);
	m_nWidth = bm.bmWidth;
	m_nHeight = bm.bmHeight;
	m_bAlphaChannel = (bm.bmBitsPixel == 32) ? TRUE : FALSE;
	AddRef();
}

UIBitmap* UIBitmap::CreateNew(HBITMAP hBitmap, LPCTSTR szCacheKey)
{
	UIBitmap* pBitmap = NULL;
	if(szCacheKey)
	{
		BitmapCache::const_iterator itor = cache.find(szCacheKey);
		if(itor != cache.end())
		{
			pBitmap = itor->second;
			pBitmap->AddRef();
		}
		else
		{
			pBitmap = new UIBitmap(hBitmap);
			pBitmap->m_szKey = szCacheKey;
			cache.insert(std::make_pair(szCacheKey, pBitmap));
		}
	}
	else
	{
		pBitmap = new UIBitmap(hBitmap);
	}
	return pBitmap;
}

UIBitmap* UIBitmap::CreateNew(int w, int h, int nBitCount, LPCVOID lpvBits, LPCTSTR szCacheKey)
{
	UIBitmap* pBitmap = NULL;
	if(szCacheKey)
	{
		BitmapCache::const_iterator itor = cache.find(szCacheKey);
		if(itor != cache.end())
		{
			pBitmap = itor->second;
			pBitmap->AddRef();
		}
		else
		{
			pBitmap = new UIBitmap(w, h, nBitCount, lpvBits);
			cache.insert(std::make_pair(szCacheKey, pBitmap));
		}
	}
	else
	{
		pBitmap = new UIBitmap(w, h, nBitCount, lpvBits);
	}
	return pBitmap;
}

BOOL UIBitmap::HasAlpha() const
{
	return m_bAlphaChannel;
}

int UIBitmap::GetWidth() const
{
	return m_nWidth;
}

int UIBitmap::GetHeight() const
{
	return m_nHeight;
}

HBITMAP UIBitmap::GetNativeBitmap() const
{
	return m_hBitmap;
}

UIBitmap::operator HBITMAP() const
{
	return m_hBitmap;
}