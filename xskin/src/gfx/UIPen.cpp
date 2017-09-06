#include "stdafx.h"
#include "../../include/gfx/UIPen.h"

UIPen::GdiPenCache UIPen::cache;

UIPen::UIPen(HPEN hPen)
{
	m_hPen = hPen;
	Init();
}

UIPen::UIPen(COLORREF clr, int nWidth)
{
	m_hPen = ::CreatePen(PS_SOLID, nWidth, clr);
	Init();
}

UIPen::~UIPen()
{
	GdiPenCache::iterator itor = cache.find(m_szKey);
	if(itor != cache.end())
		cache.erase(itor);

	if(m_hPen)
		::DeleteObject(m_hPen);
}

void UIPen::Init()
{
	AddRef();
}

UIPen* UIPen::GetPen(LPCTSTR szCacheKey)
{
	GdiPenCache::const_iterator itor = cache.find(szCacheKey);
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

UIPen* UIPen::CreateNew(HPEN hPen, LPCTSTR szCacheKey)
{
	UIPen* pPen = NULL;
	if(szCacheKey)
	{
		GdiPenCache::const_iterator itor = cache.find(szCacheKey);
		if(itor != cache.end())
		{
			pPen = itor->second;
			pPen->AddRef();
		}
		else
		{
			pPen = new UIPen(hPen);
			pPen->m_szKey = szCacheKey;
			cache.insert(std::make_pair(szCacheKey, pPen));
		}
	}
	else
	{
		pPen = new UIPen(hPen);
	}

	return pPen;
}

UIPen* UIPen::CreateNew(COLORREF clr, int nWidth, LPCTSTR szCacheKey)
{
	UIPen* pPen = NULL;
	if(szCacheKey)
	{
		GdiPenCache::const_iterator itor = cache.find(szCacheKey);
		if(itor != cache.end())
		{
			pPen = itor->second;
			pPen->AddRef();
		}
		else
		{
			pPen = new UIPen(clr, nWidth);
			pPen->m_szKey = szCacheKey;
			cache.insert(std::make_pair(szCacheKey, pPen));
		}
	}
	else
	{
		pPen = new UIPen(clr, nWidth);
	}

	return pPen;
}

HPEN UIPen::GetNativePen() const
{
	return m_hPen;
}

UIPen::operator HPEN() const
{
	return m_hPen;
}