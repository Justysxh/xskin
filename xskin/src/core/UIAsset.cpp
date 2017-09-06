#include "stdafx.h"
#include "../../include/core/UIAsset.h"
#include "../../include/core/UIApplication.h"
#include "../../include/gfx/UIBitmap.h"
#include "../../include/gfx/UIPen.h"
#include "../../include/gfx/UIBrush.h"
#include "../../../tinyxml2/tinyxml2.h"
#include "../../../unzip/ZipFile.h"
#include "../../../libpng/png.h"
#include "../../../libpng/pngstruct.h"
#include "../../../libpng/pnginfo.h"
#include "../../../libjpeg/jpeglib.h"

struct PNGImageData
{
	const void* data;
	size_t size;
	size_t offset;
};

void PngReadCallback(png_structp pngPtr, png_bytep data, png_size_t length)
{
	PNGImageData* psrc = (PNGImageData*)png_get_io_ptr(pngPtr);
	if(psrc->offset + length <= psrc->size)
	{
		memcpy(data, static_cast<const char*>(psrc->data) + psrc->offset, length);
		psrc->offset += length;
	}
	else
	{
		png_error(pngPtr, "read png from buffer failed");
	}
}

UIAsset::UIAsset()
{
	m_bLoad = FALSE;
	m_pResZip.Reset(new ZipFile);
}

UIAsset::~UIAsset()
{
	LayoutCache::iterator itor;
	while((itor = m_cacheLayout.begin()) != m_cacheLayout.end())
	{
		delete itor->second;
		m_cacheLayout.erase(itor);
	}
}

void UIAsset::XMLError(const char* szXmlFileName, const tinyxml2::XMLDocument* pDoc) const
{
	char szError[1024] = { 0 };
	sprintf(szError, "%s \n\n%s: %d", pDoc->ErrorName(), szXmlFileName, pDoc->GetErrorLineNum());
	::MessageBoxA(NULL, szError, "ERROR", MB_ICONERROR);
}

BOOL UIAsset::Load(LPCTSTR szResZipFile)
{
	char szFileName[256] = { 0 };
#if _UNICODE
	LPCSTR szTemp = UCSToANSI(szResZipFile);
	strcpy(szFileName, szTemp);
	delete szTemp;
#else
	strcpy(szFileName, szResZipFile);
#endif

	m_bLoad =  m_pResZip->Open(szFileName);
	return m_bLoad;
}

BOOL UIAsset::GetRaw(const UIString& szRawFileName, LPVOID* lppBuffer, int* pSize)
{
	if (szRawFileName.StartWidth(_T("@")))
	{
		UIString szRealFileName = szRawFileName.Mid(1, szRawFileName.GetLength() - 1);
		return LoadRawFromZip(szRealFileName, lppBuffer, pSize);
	}
	else
	{
		return LoadRawFromDisk(szRawFileName, lppBuffer, pSize);
	}
}

const tinyxml2::XMLDocument* UIAsset::GetLayout(const UIString& szXmlFileName)
{
	if (szXmlFileName.StartWidth(_T("@")))
	{
		UIString szRealFileName = szXmlFileName.Mid(1, szXmlFileName.GetLength() - 1);
		return LoadLayoutFromZip(szRealFileName);
	}
	else
	{
		return LoadLayoutFromDisk(szXmlFileName);
	}
}

const tinyxml2::XMLDocument* UIAsset::GetSkin(const UIString& szXmlFileName)
{
	if (szXmlFileName.StartWidth(_T("@")))
	{
		UIString szRealFileName = szXmlFileName.Mid(1, szXmlFileName.GetLength() - 1);
		return LoadSkinFromZip(szRealFileName);
	}
	else
	{
		return LoadSkinFromDisk(szXmlFileName);
	}
}

const UIBitmap* UIAsset::GetImage(const UIString& szImageFileName)
{
	if (szImageFileName.StartWidth(_T("@")))
	{
		UIString szRealFileName = szImageFileName.Mid(1, szImageFileName.GetLength() - 1);
		return LoadImageFromZip(szRealFileName);
	}
	else
	{
		return LoadImageFromDisk(szImageFileName);
	}
}

const UIString UIAsset::GetString(const UIString& szID)
{
	XASSERT(m_bLoad);

	std::map<UIString, UIString>::const_iterator itor = m_cacheString.find(szID);
	if(itor != m_cacheString.end())
		return itor->second;

	UIApplication::Locale locale = UIApplication::Instance()->GetLocale();
	char szInZipFile[256] = { 0 };
	if(locale == UIApplication::LOCALE_ENGLISH)
	{
		strcpy(szInZipFile, "string/string.xml");
	}
	else if(locale == UIApplication::LOCALE_SIMPLIFIED_CHINESE)
	{
		strcpy(szInZipFile, "string/string_zh.xml");
	}
	else if(locale == UIApplication::LOCALE_TRADITIONAL_CHINESE)
	{
		strcpy(szInZipFile, "string/string_tw.xml");
	}

	ZIPENTRY zi;
	if(m_pResZip->FindFile(szInZipFile, &zi))
	{
		char* szTemp = new char[zi.nUnCompressedSize];
		memset(szTemp, 0, sizeof(zi.nUnCompressedSize));
		m_pResZip->ExtractFile(&zi, szTemp, zi.nUnCompressedSize);
		tinyxml2::XMLDocument doc;
		if(doc.Parse(szTemp, zi.nUnCompressedSize) == tinyxml2::XML_SUCCESS)
		{
			tinyxml2::XMLElement* pRoot = doc.RootElement();
			tinyxml2::XMLElement* pNode = pRoot->FirstChildElement("string");
			char szAttrName[256] = { 0 };
#if _UNICODE
			LPCSTR szTemp = UCSToUTF8(szID);
#else
			LPCSTR szTemp = ANSIToUTF8(szName);
#endif
			strcpy(szAttrName, szTemp);
			delete szTemp;
			while(pNode)
			{
				if(strcmp(pNode->Attribute("id"), szAttrName) == 0)
				{
					UIString szText = UTF8ToString(pNode->GetText());
					m_cacheString.insert(std::make_pair(szID, szText));
					return szText;
				}
				pNode = pNode->NextSiblingElement();
			}
		}
		else
		{
			XMLError(szInZipFile, &doc);
		}
	}

	return TEXT("");
}

const UIFont* UIAsset::GetGuiFont() const
{
	const UIFont* pFont = UIFont::CreateNew(TEXT("Microsoft YaHei"), 9, UIFont::NORMAL, TEXT("guifont"));
	if(pFont)
		return pFont;
	
	pFont = UIFont::CreateNew(TEXT("simsun"), 12, UIFont::NORMAL, TEXT("guifont"));
	if(pFont)
		return pFont;
	else
		return NULL;
}

const UIFont* UIAsset::GetFont(const UIString& szFontName, int nFontSize, int nFontStyle) const
{
	UIString szCacheKey;
	szCacheKey.Format(TEXT("%s-%d-%d"), szFontName, nFontSize, nFontStyle);
	return UIFont::CreateNew(szFontName, nFontSize, nFontStyle, szCacheKey);
}

const UIPen* UIAsset::GetPen(COLORREF clr, int nWidth /* = 1 */) const
{
	UIString szCacheKey;
	szCacheKey.Format(TEXT("%d-%d"), clr, nWidth);
	UIPen* pPen = UIPen::GetPen(szCacheKey);
	if(pPen)
		return pPen;

	return UIPen::CreateNew(clr, nWidth, szCacheKey);
}

const UIBrush* UIAsset::GetBrush(COLORREF clr) const
{
	UIString szCacheKey;
	szCacheKey.Format(_T("%d"), clr);
	UIBrush* pBrush = UIBrush::GetBrush(szCacheKey);
	if(pBrush)
		return pBrush;

	return UIBrush::CreateNew(clr, szCacheKey);
}

const UIBitmap* UIAsset::GetRGBImage(COLORREF clr, BOOL bTrans, int alpha) const
{
	UIString szKey;
	szKey.Format(_T("RGB:%d"), clr);
	UIBitmap* pBitmap = UIBitmap::GetBitmap(szKey);
	if(pBitmap)
		return pBitmap;

	const int width = 4;
	const int height = 4;
	int nBitCount = bTrans ? 32 : 24;
	BITMAPINFOHEADER m_bmpHeader;
	memset(&m_bmpHeader, NULL, sizeof(m_bmpHeader));
	m_bmpHeader.biSize = sizeof(m_bmpHeader);
	m_bmpHeader.biBitCount = nBitCount;;
	m_bmpHeader.biCompression = BI_RGB;
	m_bmpHeader.biPlanes = 1;
	m_bmpHeader.biWidth = width;
	m_bmpHeader.biHeight = height;

	BYTE* ppvBits;
	HBITMAP hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&m_bmpHeader, DIB_RGB_COLORS, (void**)&ppvBits, NULL, 0);
	if(!hBitmap)
		return NULL;

	int nByte = nBitCount / 8;
	int nRowStride = ((width * nByte + nByte) & ~nByte);
	LPBYTE pb = static_cast<LPBYTE>(ppvBits) + nRowStride * height;
	BYTE r = GetRValue(clr);
	BYTE g = GetGValue(clr);
	BYTE b = GetBValue(clr);
	for(int y = 0; y < height; ++y)
	{
		pb -= nRowStride;
		for(int x = 0; x < nRowStride; x += nByte)
		{
			if(!bTrans)
			{
				pb[x + 0] = b;
				pb[x + 1] = g;
				pb[x + 2] = r;
			}
			else
			{
				pb[x + 0] = BYTE(b * alpha / 255);
				pb[x + 1] = BYTE(g * alpha / 255);
				pb[x + 2] = BYTE(r * alpha / 255);
				pb[x + 3] = alpha;
			}
		}
	}

	pBitmap = UIBitmap::CreateNew(hBitmap, szKey);
	return pBitmap;
}

HCURSOR UIAsset::GetCursor(CursorType type) const
{
	HCURSOR hCursor = NULL;
	if(type == CURSOR_HAND)
		hCursor = ::LoadCursor(NULL, IDC_HAND);
	else if(type == CURSOR_CROSS)
		hCursor = ::LoadCursor(NULL, IDC_CROSS);
	else if(type == CURSOR_WAIT)
		hCursor = ::LoadCursor(NULL, IDC_WAIT);
	else if(type == CURSOR_IBEAM)
		hCursor = ::LoadCursor(NULL, IDC_IBEAM);
	else 
		hCursor = ::LoadCursor(NULL, IDC_ARROW);

	return hCursor;
}

LPCWSTR UIAsset::ANSIToUCS(LPCSTR ansi)
{
	int nLen = ::MultiByteToWideChar(CP_ACP, NULL, ansi, strlen(ansi), NULL, 0);
	wchar_t* ucs = new wchar_t[nLen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, ansi, strlen(ansi), ucs, nLen);
	ucs[nLen] = '\0';
	return ucs;
}

LPCSTR UIAsset::UCSToANSI(LPCWSTR ucs)
{
	int nLen = WideCharToMultiByte(CP_ACP, NULL, ucs, wcslen(ucs), NULL, 0, NULL, NULL);
	char* ansi = new char[nLen + 1];
	WideCharToMultiByte(CP_ACP, NULL, ucs, wcslen(ucs), ansi, nLen, NULL, NULL);  
	ansi[nLen] = '\0';
	return ansi;
}

LPCSTR UIAsset::UTF8ToANSI(LPCSTR utf8)
{
	int nLen = ::MultiByteToWideChar(CP_UTF8, NULL, utf8, strlen(utf8), NULL, 0);
	wchar_t* ucs = new wchar_t[nLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, utf8, strlen(utf8), ucs, nLen);
	ucs[nLen] = '\0';
	nLen = ::WideCharToMultiByte(CP_ACP, NULL, ucs, wcslen(ucs), NULL, 0, NULL, NULL);
	char* ansi = new char[nLen + 1];
	::WideCharToMultiByte(CP_ACP, NULL, ucs, wcslen(ucs), ansi, nLen, NULL, NULL);
	ansi[nLen] = '\0';
	delete[] ucs;
	return ansi;
}

LPCWSTR UIAsset::UTF8ToUCS(LPCSTR utf8)
{
	int nLen = ::MultiByteToWideChar(CP_UTF8, NULL, utf8, strlen(utf8), NULL, 0);
	wchar_t* ucs = new wchar_t[nLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, utf8, strlen(utf8), ucs, nLen);
	ucs[nLen] = '\0';
	return ucs;
}

LPCSTR UIAsset::ANSIToUTF8(LPCSTR ansi)
{
	int nLen = ::MultiByteToWideChar(CP_ACP, NULL, ansi, strlen(ansi), NULL, 0);
	wchar_t* ucs = new wchar_t[nLen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, ansi, strlen(ansi), ucs, nLen);
	ucs[nLen] = '\0';
	nLen = ::WideCharToMultiByte(CP_UTF8, NULL, ucs, wcslen(ucs), NULL, 0, NULL, NULL);
	char* utf8 = new char[nLen + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, ucs, wcslen(ucs), utf8, nLen, NULL, NULL);
	utf8[nLen] = '\0';
	delete[] ucs;
	return utf8;
}

LPCSTR UIAsset::UCSToUTF8(LPCWSTR ucs)
{
	int nLen = WideCharToMultiByte(CP_UTF8, NULL, ucs, wcslen(ucs), NULL, 0, NULL, NULL);
	char* utf8 = new char[nLen + 1];
	WideCharToMultiByte(CP_UTF8, NULL, ucs, wcslen(ucs), utf8, nLen, NULL, NULL);
	utf8[nLen] = '\0';
	return utf8;
}

LPCSTR UIAsset::StringToUTF8(LPCTSTR str)
{
	LPCSTR szRet;
#if _UNICODE
	szRet = UCSToUTF8(str);
#else
	szRet = ANSIToUTF8(str);
#endif
	return szRet;
}

UIString UIAsset::UTF8ToString(LPCSTR str)
{
	LPCTSTR szRet;
#if _UNICODE
	szRet = UTF8ToUCS(str);
#else
	szRet = UTF8ToANSI(str);
#endif
	UIString s(szRet);
	delete szRet;
	return s;
}

HBITMAP	UIAsset::LoadPNG(const void* buffer, int size) const
{
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!pngPtr)
		return NULL;

	png_infop infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr)
		return NULL;

	if(setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, 0);
		return NULL;
	}

	PNGImageData imgdata;
	imgdata.data = buffer;
	imgdata.size = size;
	imgdata.offset = 0;
	png_set_read_fn(pngPtr, &imgdata, PngReadCallback);
	png_read_info(pngPtr, infoPtr);

	int colorType = infoPtr->color_type;
	int bitDepth = infoPtr->bit_depth;
	if(colorType == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(pngPtr);

	if(colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
		png_set_expand_gray_1_2_4_to_8(pngPtr);

	if(bitDepth == 16)
		png_set_strip_16(pngPtr);

	if(png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(pngPtr);

	if(colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(pngPtr);

	png_read_update_info(pngPtr, infoPtr);

	unsigned long x, y, i;
	BYTE r, g, b, a;
	HBITMAP hBitmap = NULL;
	png_bytep* rowPtr = (png_bytep*)malloc(sizeof(png_bytep) * infoPtr->height);
	if(rowPtr)
	{
		for(y = 0; y < infoPtr->height; y++)
		{
			rowPtr[y] = (png_byte*)malloc(infoPtr->rowbytes);
		}
		png_read_image(pngPtr, rowPtr);
		BITMAPINFOHEADER m_bmpHeader;
		memset(&m_bmpHeader, NULL, sizeof(m_bmpHeader));
		m_bmpHeader.biSize = sizeof(m_bmpHeader);
		m_bmpHeader.biBitCount = 32;;
		m_bmpHeader.biCompression = BI_RGB;
		m_bmpHeader.biWidth = infoPtr->width;
		m_bmpHeader.biHeight = infoPtr->height;
		m_bmpHeader.biPlanes = 1;

		BYTE *ppvBits;
		hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&m_bmpHeader, DIB_RGB_COLORS, (void**)&ppvBits, NULL, 0);
		if(hBitmap && ppvBits)
		{
			for(y = 0; y < infoPtr->height; y++)
			{
				memcpy(ppvBits + (infoPtr->height - y - 1)*infoPtr->rowbytes, rowPtr[y], infoPtr->rowbytes);
			}
			for(y = 0; y < infoPtr->height; y++)
			{
				for(x = 0; x < infoPtr->width; x++)
				{
					i = (infoPtr->height - 1 - y)*infoPtr->width * 4 + x * 4;
					r = ppvBits[i + 0];
					g = ppvBits[i + 1];
					b = ppvBits[i + 2];
					a = ppvBits[i + 3];

					// RGBA -> BGRA and to AlphaBend() Colors  
					ppvBits[i + 0] = BYTE(b * a / 255);
					ppvBits[i + 1] = BYTE(g * a / 255);
					ppvBits[i + 2] = BYTE(r * a / 255);
				}
			}
		}
		for(y = 0; y < infoPtr->height; y++)
		{
			free(rowPtr[y]);
		}
		free(rowPtr);
	}
	png_destroy_read_struct(&pngPtr, &infoPtr, 0);

	return hBitmap;
}

HBITMAP UIAsset::LoadBMP(const void* buffer, int size) const
{
	BITMAPFILEHEADER bif;
	memset(&bif, 0, sizeof(bif));
	memcpy(&bif, buffer, sizeof(bif));

	BITMAPINFOHEADER bih;
	memset(&bih, 0, sizeof(bih));
	memcpy(&bih, static_cast<const char*>(buffer) + sizeof(BITMAPFILEHEADER), sizeof(bih));

	int nPalSize = bih.biClrUsed * sizeof(RGBQUAD);
	char* temp = new char[sizeof(BITMAPINFOHEADER) + nPalSize];
	BITMAPINFO* pbmi = reinterpret_cast<BITMAPINFO*>(temp);
	memcpy(&pbmi->bmiHeader, &bih, sizeof(bih));
	memcpy(pbmi->bmiColors, static_cast<const char*>(buffer) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), nPalSize);

	void* pvbits = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, pbmi, DIB_RGB_COLORS, &pvbits, NULL, 0);
	delete temp;

	if(hBitmap)
	{
		memcpy(pvbits, static_cast<const char*>(buffer) + bif.bfOffBits, size - sizeof(BITMAPINFO) - sizeof(BITMAPFILEHEADER) - nPalSize);
		return hBitmap;
	}
	else
	{
		return NULL;
	}
}

HBITMAP UIAsset::LoadJPG(const void* buffer, int size) const
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, static_cast<const unsigned char*>(buffer), size);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	UINT row_stride = ((cinfo.output_width * 3 + 3) & ~3);
	JSAMPARRAY raw = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	BITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = cinfo.output_width;
	bi.bmiHeader.biHeight = cinfo.output_height;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;

	void* pvbit;
	HBITMAP hBitmap = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, &pvbit, NULL, 0);
	if (hBitmap)
	{
		LPBYTE pb = static_cast<LPBYTE>(pvbit) + row_stride * cinfo.output_height;
		while (cinfo.output_scanline < cinfo.output_height)
		{
			pb -= row_stride;
			jpeg_read_scanlines(&cinfo, raw, 1);

			if (cinfo.out_color_components == 1)
			{
				LPBYTE p = (LPBYTE)raw[0];
				for (UINT x = 0; x < cinfo.output_width; ++x)
				{
					pb[3 * x + 0] = p[x];
					pb[3 * x + 1] = p[x];
					pb[3 * x + 2] = p[x];
				}
			}
			else if (cinfo.out_color_components == 3)
			{
				LPBYTE p = (LPBYTE)raw[0];
				for (UINT x = 0; x < row_stride; x += 3)
				{
					pb[x + 0] = p[x + 2];
					pb[x + 1] = p[x + 1];
					pb[x + 2] = p[x + 0];
				}
			}
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return hBitmap;
}
const UIBitmap* UIAsset::LoadImageFromZip(const UIString& szImageFileName)
{
	XASSERT(m_bLoad);

	UIBitmap* pBitmap = UIBitmap::GetBitmap(szImageFileName);
	if (pBitmap)
		return pBitmap;

	char szInZipPath[256] = { 0 };
#if _UNICODE
	LPCSTR szTemp = UCSToANSI(szImageFileName);
	sprintf(szInZipPath, "%s", szTemp);
	delete szTemp;
#else
	sprintf(szInZipPath, "%s", szImageFileName);
#endif

	ZIPENTRY zi;
	if (m_pResZip->FindFile(szInZipPath, &zi))
	{
		char* szTemp = new char[zi.nUnCompressedSize];
		memset(szTemp, 0, sizeof(zi.nUnCompressedSize));
		m_pResZip->ExtractFile(&zi, szTemp, zi.nUnCompressedSize);

		HBITMAP hBitmap;
		if (szImageFileName.EndWith(_T(".bmp")))
			hBitmap = LoadBMP(szTemp, zi.nUnCompressedSize);
		else if (szImageFileName.EndWith(_T(".png")))
			hBitmap = LoadPNG(szTemp, zi.nUnCompressedSize);
		else if (szImageFileName.EndWith(_T(".jpg")))
			hBitmap = LoadJPG(szTemp, zi.nUnCompressedSize);
		else
			hBitmap = NULL;

		delete szTemp;
		if (hBitmap)
		{
			UIBitmap* pBitmap = UIBitmap::CreateNew(hBitmap, szImageFileName);
			return pBitmap;
		}
	}

	return NULL;
}

const UIBitmap* UIAsset::LoadImageFromDisk(const UIString& szImageFileName)
{
	UIBitmap* pBitmap = UIBitmap::GetBitmap(szImageFileName);
	if (pBitmap)
		return pBitmap;

	FILE* fp = _tfopen(szImageFileName, _T("rb"));
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* szTemp = new char[nSize];
		memset(szTemp, 0, nSize);
		fread(szTemp, 1, nSize, fp);
		fclose(fp);

		HBITMAP hBitmap;
		if (szImageFileName.EndWith(_T(".bmp")))
			hBitmap = LoadBMP(szTemp, nSize);
		else if (szImageFileName.EndWith(_T(".png")))
			hBitmap = LoadPNG(szTemp, nSize);
		else if (szImageFileName.EndWith(_T(".jpg")))
			hBitmap = LoadJPG(szTemp, nSize);
		else
			hBitmap = NULL;

		delete szTemp;
		if (hBitmap)
		{
			UIBitmap* pBitmap = UIBitmap::CreateNew(hBitmap, szImageFileName);
			return pBitmap;
		}
	}

	return NULL;
}

BOOL UIAsset::LoadRawFromZip(const UIString& szRawFileName, LPVOID* lppBuffer, int* pSize)
{
	XASSERT(m_bLoad);
	XASSERT(lppBuffer);

	char szInZipPath[256] = { 0 };
#if _UNICODE
	LPCSTR szTemp = UCSToANSI(szRawFileName);
	sprintf(szInZipPath, "%s", szTemp);
	delete szTemp;
#else
	sprintf(szInZipPath, "%s", szImageFileName);
#endif

	ZIPENTRY zi;
	if (m_pResZip->FindFile(szInZipPath, &zi))
	{
		void* buffer = new char[zi.nUnCompressedSize];
		memset(buffer, 0, sizeof(zi.nUnCompressedSize));
		m_pResZip->ExtractFile(&zi, buffer, zi.nUnCompressedSize);
		*lppBuffer = buffer;
		*pSize = zi.nUnCompressedSize;
		return TRUE;
	}

	return FALSE;
}

BOOL UIAsset::LoadRawFromDisk(const UIString& szRawFileName, LPVOID* lppBuffer, int* pSize)
{
	XASSERT(lppBuffer);

	FILE* fp = _tfopen(szRawFileName, _T("rb"));
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* szTemp = new char[nSize];
		memset(szTemp, 0, nSize);
		fread(szTemp, 1, nSize, fp);
		fclose(fp);

		*lppBuffer = szTemp;
		*pSize = nSize;
		return TRUE;
	}

	return FALSE;
}

const tinyxml2::XMLDocument* UIAsset::LoadLayoutFromZip(const UIString& szXmlFileName)
{
	XASSERT(m_bLoad);

	LayoutCache::const_iterator itor = m_cacheLayout.find(szXmlFileName);
	if (itor != m_cacheLayout.end())
		return itor->second;

	char szInZipPath[256] = { 0 };
#if _UNICODE
	LPCSTR szTemp = UCSToANSI(szXmlFileName);
	sprintf(szInZipPath, "%s", szTemp);
	delete szTemp;
#else
	sprintf(szInZipPath, "%s", szXmlFileName);
#endif
	ZIPENTRY zi;
	if (m_pResZip->FindFile(szInZipPath, &zi))
	{
		char* szTemp = new char[zi.nUnCompressedSize];
		memset(szTemp, 0, sizeof(zi.nUnCompressedSize));
		m_pResZip->ExtractFile(&zi, szTemp, zi.nUnCompressedSize);
		tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument;
		if (pDoc->Parse(szTemp, zi.nUnCompressedSize) == tinyxml2::XML_SUCCESS)
		{
			m_cacheLayout.insert(std::make_pair(szXmlFileName, pDoc));
			return pDoc;
		}
		else
		{
			XMLError(szInZipPath, pDoc);
		}
	}

	return NULL;
}

const tinyxml2::XMLDocument* UIAsset::LoadLayoutFromDisk(const UIString& szXmlFileName)
{
	LayoutCache::const_iterator itor = m_cacheLayout.find(szXmlFileName);
	if (itor != m_cacheLayout.end())
		return itor->second;

	char szFilePath[256] = { 0 };
#if _UNICODE
	LPCSTR temp = UCSToANSI(szXmlFileName);
	sprintf(szFilePath, "%s", temp);
	delete temp;
#else
	sprintf(szFilePath, "%s", szXmlFileName);
#endif

	FILE* fp = _tfopen(szXmlFileName, _T("rb"));
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* szTemp = new char[nSize];
		memset(szTemp, 0, nSize);
		fread(szTemp, 1, nSize, fp);
		fclose(fp);

		tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument;
		if (pDoc->Parse(szTemp, nSize) == tinyxml2::XML_SUCCESS)
		{
			m_cacheLayout.insert(std::make_pair(szXmlFileName, pDoc));
			return pDoc;
		}
		else
		{
			XMLError(szFilePath, pDoc);
		}
	}

	return NULL;
}

const tinyxml2::XMLDocument* UIAsset::LoadSkinFromZip(const UIString& szXmlFileName)
{
	XASSERT(m_bLoad);

	SkinCache::const_iterator itor = m_cachedSkin.find(szXmlFileName);
	if (itor != m_cachedSkin.end())
		return itor->second;

	char szInZipPath[256] = { 0 };
#if _UNICODE
	LPCSTR szTemp = UCSToANSI(szXmlFileName);
	sprintf(szInZipPath, "%s", szTemp);
	delete szTemp;
#else
	sprintf(szInZipPath, "%s", szXmlFileName);
#endif
	ZIPENTRY zi;
	if (m_pResZip->FindFile(szInZipPath, &zi))
	{
		char* szTemp = new char[zi.nUnCompressedSize];
		memset(szTemp, 0, sizeof(zi.nUnCompressedSize));
		m_pResZip->ExtractFile(&zi, szTemp, zi.nUnCompressedSize);
		tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument;
		if (pDoc->Parse(szTemp, zi.nUnCompressedSize) == tinyxml2::XML_SUCCESS)
		{
			m_cachedSkin.insert(std::make_pair(szXmlFileName, pDoc));
			return pDoc;
		}
		else
		{
			XMLError(szInZipPath, pDoc);
		}
	}

	return NULL;
}

const tinyxml2::XMLDocument* UIAsset::LoadSkinFromDisk(const UIString& szXmlFileName)
{
	SkinCache::const_iterator itor = m_cachedSkin.find(szXmlFileName);
	if (itor != m_cachedSkin.end())
		return itor->second;

	char szFileName[256] = { 0 };
#if _UNICODE
	LPCSTR szTemp = UCSToANSI(szXmlFileName);
	sprintf(szFileName, "%s", szTemp);
	delete szTemp;
#else
	sprintf(szFileName, "%s", szXmlFileName);
#endif
	
	FILE* fp = _tfopen(szXmlFileName, _T("rb"));
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size_t nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* szTemp = new char[nSize];
		memset(szTemp, 0, nSize);
		fread(szTemp, 1, nSize, fp);
		fclose(fp);

		tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument;
		if (pDoc->Parse(szTemp, nSize) == tinyxml2::XML_SUCCESS)
		{
			m_cachedSkin.insert(std::make_pair(szXmlFileName, pDoc));
			return pDoc;
		}
		else
		{
			XMLError(szFileName, pDoc);
		}
	}
	return NULL;
}
