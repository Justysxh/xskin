#ifndef __UIAPPLICATION_H_INCLUDED__
#define __UIAPPLICATION_H_INCLUDED__

#include <string>
#include "UIResponser.h"
#include "UIAsset.h"
#include "UIString.h"
#include "AutoPtr.h"

class XSKIN_API UIApplication : public UIResponser
{
public:
	enum Locale
	{
		LOCALE_FOLLOW_SYSTEM,
		LOCALE_ENGLISH,
		LOCALE_SIMPLIFIED_CHINESE,
		LOCALE_TRADITIONAL_CHINESE
	};

public:
							UIApplication();
							~UIApplication();
	static UIApplication*	Instance();
	int						Run() const;
	void					Exit(int nRetuenCode = 0) const;
	void					SetResZip(LPCTSTR szResZipFile);
	UIAsset*				GetAssetManager() const;
	int						GetCommandCount() const;
	UIString				GetCommandString(int nIndex) const;
	UIString				GetApplicationName() const;
	void					SetLocale(Locale locale);
	Locale					GetLocale() const;
	HINSTANCE				GetHandle() const;
	static void				SendEvent(UIEvent* event);

protected:
	BOOL					Event(UIEvent* event) OVERRIDE;

protected:
	static UIApplication* self;
	AutoPtr<UIAsset> m_pAsset;
	Locale m_locale;
};

#endif //! __UIAPPLICATION_H_INCLUDED__