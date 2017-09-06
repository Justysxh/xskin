#ifndef __UIASSET_H_INCLUDED__
#define __UIASSET_H_INCLUDED__

#include <map>
#include "UIObject.h"
#include "../core/AutoPtr.h"
#include "../core/UIString.h"
#include "../gfx/UIFont.h"

namespace tinyxml2 { class XMLDocument; };
class UIPen;
class UIBrush;
class UIBitmap;
class ZipFile;
class XSKIN_API UIAsset : public UIObject
{
public:
	enum CursorType
	{
		CURSOR_ARROW = 0,
		CURSOR_HAND,
		CURSOR_CROSS,
		CURSOR_IBEAM,
		CURSOR_WAIT
	};
									UIAsset();
									~UIAsset();

public:
	BOOL							Load(LPCTSTR szResZipFile);
	BOOL							GetRaw(const UIString& szRawFileName, LPVOID* lppBuffer, int* pSize);
	const tinyxml2::XMLDocument*	GetLayout(const UIString& szXmlFileName);
	const tinyxml2::XMLDocument*	GetSkin(const UIString& szXmlFileName);
	const UIBitmap*					GetImage(const UIString& szImageFileName);
	const UIString					GetString(const UIString& szID);

	const UIBitmap*					GetRGBImage(COLORREF clr, BOOL bTrans = FALSE, int alpha = 255) const;
	const UIFont*					GetGuiFont() const;
	const UIFont*					GetFont(const UIString& szFontName, int nFontSize, int nFontStyle) const;
	const UIPen*					GetPen(COLORREF clr, int nWidth = 1) const;
	const UIBrush*					GetBrush(COLORREF clr) const;
	HCURSOR							GetCursor(CursorType type) const;

	static UIString					UTF8ToString(LPCSTR str);
	static LPCWSTR					ANSIToUCS(LPCSTR ansi);
	static LPCSTR					ANSIToUTF8(LPCSTR ansi);
	static LPCSTR					UCSToANSI(LPCWSTR ucs);
	static LPCSTR					UCSToUTF8(LPCWSTR ucs);
	static LPCSTR					UTF8ToANSI(LPCSTR utf8);
	static LPCWSTR					UTF8ToUCS(LPCSTR utf8);
	static LPCSTR					StringToUTF8(LPCTSTR str);
	
protected:
	HBITMAP							LoadPNG(const void* buffer, int size) const;
	HBITMAP							LoadBMP(const void* buffer, int size) const;
	HBITMAP							LoadJPG(const void* buffer, int size) const;
	const UIBitmap*					LoadImageFromZip(const UIString& szImageFileName);
	const UIBitmap*					LoadImageFromDisk(const UIString& szImageFileName);
	BOOL							LoadRawFromZip(const UIString& szRawFileName, LPVOID* lppBuffer, int* pSize);
	BOOL							LoadRawFromDisk(const UIString& szRawFileName, LPVOID* lppBuffer, int* pSize);
	const tinyxml2::XMLDocument*	LoadLayoutFromZip(const UIString& szXmlFileName);
	const tinyxml2::XMLDocument*	LoadLayoutFromDisk(const UIString& szXmlFileName);
	const tinyxml2::XMLDocument*	LoadSkinFromZip(const UIString& szXmlFileName);
	const tinyxml2::XMLDocument*	LoadSkinFromDisk(const UIString& szXmlFileName);
	void							XMLError(const char* szXmlFileName, const tinyxml2::XMLDocument* pDoc) const;
	
private:
	BOOL m_bLoad;
	AutoPtr<ZipFile> m_pResZip;
	typedef std::map<UIString, tinyxml2::XMLDocument*> LayoutCache;
	typedef std::map<UIString, tinyxml2::XMLDocument*> SkinCache;
	typedef std::map<UIString, UIString> StringCache;
	LayoutCache m_cacheLayout;
	SkinCache m_cachedSkin;
	StringCache m_cacheString;
};

#endif //! __UIASSET_H_INCLUDED__