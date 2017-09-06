#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "../../include/core/UIApplication.h"

UIApplication* UIApplication::self = NULL;

UIApplication::UIApplication()
{
	if(self == NULL)
	{
		self = this;
		SetLocale(LOCALE_ENGLISH);
		m_pAsset.Reset(new UIAsset);
	}
}

BOOL UIApplication::Event(UIEvent* event)
{
	return FALSE;
}

UIApplication::~UIApplication()
{
	
}

UIApplication* UIApplication::Instance()
{
	return self;
}

int UIApplication::Run() const
{
	MSG msg = { 0 };
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}

void UIApplication::Exit(int nRetuenCode) const
{
	
	::PostQuitMessage(nRetuenCode);
}

void UIApplication::SetLocale(Locale locale)
{
	if(locale == LOCALE_FOLLOW_SYSTEM)
	{
		WORD wLangID = PRIMARYLANGID(::GetSystemDefaultLangID());
		switch(wLangID)
		{
		case LANG_CHINESE_SIMPLIFIED:
			m_locale = LOCALE_SIMPLIFIED_CHINESE;
			break;
		case LANG_CHINESE_TRADITIONAL:
			m_locale = LOCALE_TRADITIONAL_CHINESE;
		default:
			m_locale = LOCALE_ENGLISH;
		}
	}
	else
	{
		m_locale = locale;
	}
}

UIApplication::Locale UIApplication::GetLocale() const
{
	return m_locale;
}

HINSTANCE UIApplication::GetHandle() const
{
	return ::GetModuleHandle(NULL);
}

void UIApplication::SendEvent(UIEvent* event)
{
	UIResponser* p = event->GetSource();
	BOOL bComsumed = FALSE;
	for(; p && !bComsumed; p = p->GetNextResponser())
	{
		bComsumed = p->Event(event);
	}
}

int UIApplication::GetCommandCount() const
{
	return __argc;
}

UIString UIApplication::GetCommandString(int nIndex) const
{
	XASSERT(nIndex < __argc);
	return __targv[nIndex];
}

UIString UIApplication::GetApplicationName() const
{
	return __targv[0];
}

UIAsset* UIApplication::GetAssetManager() const
{
	return m_pAsset.Get();
}

void UIApplication::SetResZip(LPCTSTR szResZipFile)
{
	BOOL bRet = m_pAsset->Load(szResZipFile);
	XASSERT(bRet);
}