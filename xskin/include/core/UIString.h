#ifndef __UISTRING_H_INCLUDED__
#define __UISTRING_H_INCLUDED__

#include <windows.h>
#include "UIObject.h"

class XSKIN_API UIString
{
public:
	enum { INIT_STRING_MAX_LEN = 64 };

					UIString();
					UIString(const TCHAR ch);
					UIString(LPCTSTR lpszString, int nLen = -1);
					UIString(const UIString& str);
					~UIString();

public:
	void			Empty();
	int				GetLength() const;
	BOOL			IsEmpty() const;
	TCHAR			GetAt(int nIndex) const;
	void			SetAt(int nIndex, TCHAR ch);
	UIString&		Append(LPCTSTR lpszString);
	void			Assign(LPCTSTR lpszString, int nLen = -1);
	LPTSTR			GetData() const;
	int				Compare(LPCTSTR lpszString) const;
	int				CompareNoCase(LPCTSTR lpszString) const;
	void			MakeUpper();
	void			MakeLower();
	void			Trim();
	void			TrimLeft();
	void			TrimRight();
	UIString		Tokenize(LPCTSTR szTokens, int& nStart) const;
	BOOL			StartWidth(const LPCTSTR str) const;
	BOOL			EndWith(const LPCTSTR str) const;
	UIString		Left(int nLen) const;
	UIString		Mid(int nPos, int nLen = -1) const;
	UIString		Right(int nLen) const;
	int				Find(TCHAR ch, int nPos = 0) const;
	int				Find(LPCTSTR lpszString, int nPos = 0) const;
	int				ReverseFind(TCHAR ch) const;
	int				Replace(LPCTSTR lpszStringFrom, LPCTSTR lpszStringTo);
	void			Format(LPCTSTR lpszFormat, ...);

	operator		LPCTSTR() const;
	TCHAR			operator[](int nIndex) const;
	const UIString&	operator=(const UIString& str);
	const UIString&	operator=(const TCHAR ch);
	const UIString&	operator=(LPCTSTR lpszString);
	const UIString&	operator+=(const UIString& str);
	const UIString&	operator+=(LPCTSTR lpszString);
	const UIString&	operator+=(const TCHAR ch);
	const UIString	operator+(const UIString& str) const;
	const UIString	operator+(LPCTSTR lpszString) const;
	const UIString	operator+(const TCHAR ch) const;
	BOOL			operator==(LPCTSTR str) const;
	BOOL			operator!=(LPCTSTR str) const;
	BOOL			operator<=(LPCTSTR str) const;
	BOOL			operator<(LPCTSTR str) const;
	BOOL			operator>=(LPCTSTR str) const;
	BOOL			operator>(LPCTSTR str) const;

private:
	LPTSTR m_lpszData;
};
#endif //! __UISTRING_H_INCLUDED__