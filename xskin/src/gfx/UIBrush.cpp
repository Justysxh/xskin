#include "stdafx.h"
#include "../../include/gfx/UIBrush.h"

UIBrush::GdiBrushCache UIBrush::cache;

UIBrush::UIBrush(HBRUSH hBrush)
{
	m_hBrush = hBrush;
	Init();
}

UIBrush::UIBrush(COLORREF clr)
{
	m_hBrush = ::CreateSolidBrush(clr);
	Init();
}

UIBrush::~UIBrush()
{
	GdiBrushCache::iterator itor = cache.find(m_szKey);
	if(itor != cache.end())
		cache.erase(itor);

	if(m_hBrush)
		::DeleteObject(m_hBrush);
}

void UIBrush::Init()
{
	AddRef();
}

UIBrush* UIBrush::GetBrush(LPCTSTR szCacheKey)
{
	GdiBrushCache::const_iterator itor = cache.find(szCacheKey);
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

UIBrush* UIBrush::CreateNew(HBRUSH hBrush, LPCTSTR szCacheKey)
{
	UIBrush* pBrush = NULL;
	if(szCacheKey)
	{
		GdiBrushCache::const_iterator itor = cache.find(szCacheKey);
		if(itor != cache.end())
		{
			pBrush = itor->second;
			pBrush->AddRef();
		}
		else
		{
			pBrush = new UIBrush(hBrush);
			pBrush->m_szKey = szCacheKey;
			cache.insert(std::make_pair(szCacheKey, pBrush));
		}
	}
	else
	{
		pBrush = new UIBrush(hBrush);
	}

	return pBrush;
}

UIBrush* UIBrush::CreateNew(COLORREF clr, LPCTSTR szCacheKey)
{
	UIBrush* pBrush = NULL;
	if(szCacheKey)
	{
		GdiBrushCache::const_iterator itor = cache.find(szCacheKey);
		if(itor != cache.end())
		{
			pBrush = itor->second;
			pBrush->AddRef();
		}
		else
		{
			pBrush = new UIBrush(clr);
			pBrush->m_szKey = szCacheKey;
			cache.insert(std::make_pair(szCacheKey, pBrush));
		}
	}
	else
	{
		pBrush = new UIBrush(clr);
	}

	return pBrush;
}

HBRUSH UIBrush::GetNativeBrush() const
{
	return m_hBrush;
}

UIBrush::operator HBRUSH() const
{
	return m_hBrush;
}