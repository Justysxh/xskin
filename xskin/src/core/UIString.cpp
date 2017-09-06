#include "stdafx.h"
#include <tchar.h>
#include "../../include/core/UIString.h"
#include "../../include/core/UIObject.h"

UIString::UIString()
{
	m_lpszData = new TCHAR[INIT_STRING_MAX_LEN + 1];
	memset(static_cast<void*>(m_lpszData), 0, (INIT_STRING_MAX_LEN + 1) * sizeof(TCHAR));
}

UIString::UIString(const TCHAR ch)
{
	m_lpszData = new TCHAR[INIT_STRING_MAX_LEN + 1];
	memset(static_cast<void*>(m_lpszData), 0, (INIT_STRING_MAX_LEN + 1) * sizeof(TCHAR));
	m_lpszData[0] = ch;
}

UIString::UIString(LPCTSTR lpszString, int nLen/* = -1 */)
{
	m_lpszData = new TCHAR[INIT_STRING_MAX_LEN + 1];
	memset(static_cast<void*>(m_lpszData), 0, (INIT_STRING_MAX_LEN + 1) * sizeof(TCHAR));
	XASSERT(!::IsBadStringPtr(m_lpszData, -1));
	Assign(lpszString, nLen);
}

UIString::UIString(const UIString& str)
{
	m_lpszData = new TCHAR[INIT_STRING_MAX_LEN + 1];
	memset(static_cast<void*>(m_lpszData), 0, (INIT_STRING_MAX_LEN + 1) * sizeof(TCHAR));
	Assign(str.m_lpszData);
}

UIString::~UIString()
{
	delete[] m_lpszData;
}

void UIString::Empty()
{
	delete[] m_lpszData;
	m_lpszData = new TCHAR[INIT_STRING_MAX_LEN + 1];
	memset(m_lpszData, 0, (INIT_STRING_MAX_LEN + 1) * sizeof(TCHAR));
}

int UIString::GetLength() const
{
	return _tcslen(m_lpszData);
}

BOOL UIString::IsEmpty() const
{
	return m_lpszData[0] == 0;
}

TCHAR UIString::GetAt(int nIndex) const
{
	XASSERT(nIndex < GetLength());
	return m_lpszData[nIndex];
}

void UIString::SetAt(int nIndex, TCHAR ch)
{
	XASSERT(nIndex < GetLength());
	m_lpszData[nIndex] = ch;
}

UIString& UIString::Append(LPCTSTR lpszString)
{
	int nNewLen = max(INIT_STRING_MAX_LEN, GetLength() + _tcslen(lpszString));
	if(nNewLen <= INIT_STRING_MAX_LEN)
	{
		_tcscat(m_lpszData, lpszString);
	}
	else
	{
		TCHAR* pNewData = new TCHAR[nNewLen + 1];
		memset(pNewData, 0, (nNewLen + 1) * sizeof(TCHAR));
		_tcscpy(pNewData, m_lpszData);
		_tcscat(pNewData, lpszString);
		delete[] m_lpszData;
		m_lpszData = pNewData;
	}
	return *this;
}

void UIString::Assign(LPCTSTR lpszString, int nLen/* = -1 */)
{
	nLen = nLen < 0 ? _tcslen(lpszString) : nLen;
	if(nLen > INIT_STRING_MAX_LEN)
	{
		delete[] m_lpszData;
		m_lpszData = new TCHAR[nLen + 1];
		memset(m_lpszData, 0, (nLen + 1) * sizeof(TCHAR));
	}
	_tcsncpy(m_lpszData, lpszString, nLen);
	m_lpszData[nLen] = '\0';
}

LPTSTR UIString::GetData() const
{
	return m_lpszData;
}

UIString::operator LPCTSTR()const
{
	return m_lpszData;
}

TCHAR UIString::operator[](int nIndex) const
{
	return m_lpszData[nIndex];
}

const UIString& UIString::operator=(const UIString& str)
{
	Assign((LPCTSTR)str);
	return *this;
}
const UIString& UIString::operator=(const TCHAR ch)
{
	Empty();
	m_lpszData[0] = ch;
	return *this;
}
const UIString& UIString::operator=(LPCTSTR lpszString)
{
	XASSERT(!::IsBadStringPtr(lpszString, -1));
	Assign((LPCTSTR)lpszString);
	return *this;
}
const UIString UIString::operator+(const UIString& str) const
{
	UIString strTemp = *this;
	strTemp.Append((LPCTSTR)str);
	return strTemp;
}
const UIString UIString::operator+(LPCTSTR lpszString) const
{
	UIString strTemp = *this;
	strTemp.Append(lpszString);
	return strTemp;
}
const UIString UIString::operator+(const TCHAR ch) const
{
	TCHAR str[] = { ch, '\0' };
	UIString strTemp = *this;
	strTemp.Append(str);
	return strTemp;
}

const UIString& UIString::operator+=(const UIString& str)
{
	Append((LPCTSTR)str);
	return *this;
}
const UIString& UIString::operator +=(LPCTSTR lpszString)
{
	XASSERT(!::IsBadStringPtr(lpszString, -1));
	Append(lpszString);
	return *this;
}
const UIString& UIString::operator+=(const TCHAR ch)
{
	TCHAR str[] = { ch, '\0' };
	Append(str);
	return *this;
}

BOOL UIString::operator == (LPCTSTR str) const
{
	return (Compare(str) == 0);
}

BOOL UIString::operator != (LPCTSTR str) const
{
	return (Compare(str) != 0);
}

BOOL UIString::operator <= (LPCTSTR str) const
{
	return (Compare(str) <= 0);
}

BOOL UIString::operator <  (LPCTSTR str) const
{
	return (Compare(str) < 0);
}

BOOL UIString::operator >= (LPCTSTR str) const
{
	return (Compare(str) >= 0);
}

BOOL UIString::operator >  (LPCTSTR str) const
{
	return (Compare(str) > 0);
}

int UIString::Compare(LPCTSTR lpszString) const
{
	return _tcscmp(m_lpszData, lpszString);
}

int UIString::CompareNoCase(LPCTSTR lpszString) const
{
	return _tcsicmp(m_lpszData, lpszString);
}

void UIString::MakeUpper()
{
	_tcsupr(m_lpszData);
}

void UIString::MakeLower()
{
	_tcslwr(m_lpszData);
}

void UIString::Trim()
{
	TrimLeft();
	TrimRight();
}

void UIString::TrimLeft()
{
	LPCTSTR lpsz = m_lpszData;
	while(_istspace(*lpsz))
		lpsz = _tcsinc(lpsz);

	if(lpsz != m_lpszData)
	{
		int nDataLength = GetLength() - (lpsz - m_lpszData);
		memmove(m_lpszData, lpsz, (nDataLength + 1) * sizeof(TCHAR));
	}
}

void UIString::TrimRight()
{
	LPTSTR lpsz = m_lpszData;
	LPTSTR lpszLast = NULL;
	while(*lpsz != '\0')
	{
		if(_istspace(*lpsz))
		{
			if(lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
		{
			lpszLast = NULL;
		}
		lpsz = _tcsinc(lpsz);
	}

	if(lpszLast != NULL)
		*lpszLast = '\0';
}

UIString UIString::Tokenize(LPCTSTR szTokens, int& nStart) const
{
	XASSERT(nStart >= 0);
	if((szTokens == NULL))
	{
		if(nStart < GetLength())
		{
			return(UIString(GetData() + nStart));
		}
	}
	else
	{
		LPCTSTR szPlace = GetData() + nStart;
		LPCTSTR szEnd = GetData() + GetLength();
		if(szPlace < szEnd)
		{
			int nIncluding = _tcsspn(szPlace, szTokens);
			if((szPlace + nIncluding) < szEnd)
			{
				szPlace += nIncluding;
				int nExcluding = _tcscspn(szPlace, szTokens);
				int nFrom = nStart + nIncluding;
				int nUntil = nExcluding;
				nStart = nFrom + nUntil + 1;
				return(Mid(nFrom, nUntil));
			}
		}
	}
	nStart = -1;

	return UIString();
}

BOOL UIString::StartWidth(const LPCTSTR str) const
{
	return Find(str) == 0;
}

BOOL UIString::EndWith(const LPCTSTR str) const
{
	int nLen = GetLength() - _tcslen(str);
	if(nLen < 0)
		return FALSE;

	return Find(str, nLen) == nLen;
}

UIString UIString::Left(int nLen) const
{
	XASSERT(nLen >= 0 && nLen <= GetLength());
	if(nLen < 0) nLen = 0;
	if(nLen > GetLength()) nLen = GetLength();
	return UIString(m_lpszData, nLen);
}

UIString UIString::Mid(int nPos, int nLen/*= -1*/) const
{
	XASSERT(nLen <= GetLength() - nPos);
	if(nLen < 0) nLen = GetLength() - nPos;
	if(nPos + nLen > GetLength()) nLen = GetLength() - nPos;
	return UIString(m_lpszData + nPos, nLen);
}

UIString UIString::Right(int nLen) const
{
	XASSERT(nLen >= 0 && nLen <= GetLength());
	int nPos = GetLength() - nLen;
	if(nPos < 0)
	{
		nPos = 0;
		nLen = GetLength();
	}
	return UIString(m_lpszData + nPos, nLen);
}

int UIString::Find(TCHAR ch, int nPos/* = 0*/) const
{
	XASSERT(nPos >= 0 && nPos < GetLength());
	if(nPos < 0 || nPos > GetLength()) return -1;
	LPCTSTR p = _tcschr(m_lpszData + nPos, ch);
	if(p == NULL) return -1;
	return p - m_lpszData;
}

int UIString::Find(LPCTSTR lpszString, int nPos/* = 0*/) const
{
	XASSERT(!::IsBadStringPtr(lpszString, -1));
	XASSERT(nPos >= 0 && nPos < GetLength());
	if(nPos < 0 || nPos > GetLength()) return -1;
	LPCTSTR p = _tcsstr(m_lpszData + nPos, lpszString);
	if(p == NULL) return -1;
	return p - m_lpszData;
}

int UIString::ReverseFind(TCHAR ch) const
{
	LPCTSTR p = _tcsrchr(m_lpszData, ch);
	if(p == NULL) return -1;
	return p - m_lpszData;
}

int UIString::Replace(LPCTSTR lpszStringFrom, LPCTSTR lpszStringTo)
{
	UIString strTemp;
	int nCount = 0;
	int nPos = Find(lpszStringFrom);
	if(nPos < 0) return 0;
	int nFromLen = (int)_tcslen(lpszStringFrom);
	int nToLen = (int)_tcslen(lpszStringTo);
	while(nPos >= 0)
	{
		strTemp = Left(nPos);
		strTemp += lpszStringTo;
		strTemp += Mid(nPos + nFromLen);
		Assign(strTemp);
		int nPos2 = nPos + nToLen;
		if(nPos + nToLen >= GetLength())
			nPos2 = GetLength() - 1;
		nPos = Find(lpszStringFrom, nPos2);
		nCount++;
	}
	return nCount;
}

void UIString::Format(LPCTSTR lpszFormat, ...)
{
	// NOTE: The maximum size of the buffer is 1024 bytes.
	TCHAR lpszBuffer[1024] = { 0 };
	va_list argList;
	va_start(argList, lpszFormat);
	int iRet = ::wvsprintf(lpszBuffer, lpszFormat, argList);
	va_end(argList);
	Assign(lpszBuffer);
}
