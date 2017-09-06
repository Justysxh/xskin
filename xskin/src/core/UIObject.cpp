#include "stdafx.h"
#include <typeinfo.h>
#include "../../Include/core/UIObject.h"

UIObject::UIObject()
{
	
}

UIObject::~UIObject()
{

}

void UIObject::Trace(LPCTSTR szFmt, ...)
{
#if _DEBUG
	TCHAR szMsg[128] = { 0 };
	TCHAR szTime[32] = { 0 };
	SYSTEMTIME st;
	::GetLocalTime(&st);
	_sntprintf(szTime, sizeof(szTime) / sizeof(TCHAR), TEXT("%04d-%02d-%02d %02d:%02d:%02d.%03d   "),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	_tcscpy(szMsg, szTime);
	va_list arglist;
	va_start(arglist, szFmt);
	_vsntprintf(szMsg + _tcslen(szTime), sizeof(szMsg) / sizeof(TCHAR) - _tcslen(szTime), szFmt, arglist);
	_tcscat(szMsg, TEXT("\n"));
	va_end(arglist);
	::OutputDebugString(szMsg);
#endif
}

LPCSTR UIObject::Name() const
{
	return typeid(*this).name();
}

int UIObject::HashCode() const
{
	return typeid(*this).hash_code();
}

BOOL UIObject::operator==(const UIObject& obj) const
{
	return typeid(*this) == typeid(obj) ? TRUE : FALSE;
}