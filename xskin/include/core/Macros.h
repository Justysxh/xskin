#ifndef __MACROS_H_INCLUDED__
#define __MACROS_H_INCLUDED__

#include <Windows.h>
#include <crtdbg.h>

#ifdef _DLL_EXPORT
#define XSKIN_API __declspec(dllexport)
#else
#define XSKIN_API __declspec(dllimport)
#endif

#define OVERRIDE
#define XASSERT(exp) _ASSERT(exp)
#define XCLAMP(x, a, b) (x < a ? a : (x > b ? b : x))
#define XTRACE(...) Trace(__VA_ARGS__)

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

// associate xml node with view subclass.
class UIView;
typedef UIView* (*CreateView)();
typedef struct XSKIN_API _XMLNodeInfo
{
	_XMLNodeInfo(LPCTSTR cls_name, CreateView pfn);
	static UIView* CreateNode(LPCTSTR szNodeName);
	static _XMLNodeInfo* pFirst;
	_XMLNodeInfo* pNext;
	CreateView pFunc;
	LPCTSTR szClassName;
} XML_NODE_INFO;

#define DECLARE_XML_NODE(cls_name) \
public:\
	static const XML_NODE_INFO node##cls_name;\
	static UIView* CreateObject();\
	virtual LPCTSTR GetNodeName() const;

#define IMPLEMENT_XML_NODE(cls_name) \
	const XML_NODE_INFO cls_name::node##cls_name(_T(#cls_name), cls_name::CreateObject);\
	UIView* cls_name::CreateObject() { return new cls_name; }\
	LPCTSTR cls_name::GetNodeName() const { return  node##cls_name.szClassName; }

// assign xml node attribute to view.
#define DECLARE_XML_PROPERTY() \
protected:\
	void SetAttribute(const UIString& szAttrName, const UIString& szAttrValue);

#define BEGIN_XML_PROPERTY(cls_name) \
	void cls_name::SetAttribute(const UIString& szAttrName, const UIString& szAttrValue) \
	{\
		if(szAttrName == _T("")){}

#define END_XML_PROPERTY() \
	else { __super::SetAttribute(szAttrName, szAttrValue); } }

#define SET_STRING(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		if(szAttrValue.StartWidth(_T("@")))\
		{\
			UIString szRealName = szAttrValue.Mid(1, szAttrValue.GetLength() - 1);\
			obj_var_name = UIApplication::Instance()->GetAssetManager()->GetString(szRealName);\
		}\
		else\
		{\
			obj_var_name = szAttrValue;\
		}\
	}

#define SET_DRAWABLE(xml_attr_name, obj_var_name)\
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		UIString szValue = szAttrValue;\
		szValue.Trim();\
		if(szValue.StartWidth(_T("#")))\
		{\
			int nLen = szValue.GetLength();\
			if(szValue.GetLength() >= sizeof(_T("#FFFFFF")) / sizeof(TCHAR) - 1)\
			{\
				int nPos = szValue.Find(_T("#"));\
				if(nPos != -1)\
				{\
					szValue.MakeUpper();\
					int r=0,g=0,b=0,a=0;\
					BOOL bTrans = FALSE;\
					if(nLen >= 9)\
					{\
						bTrans = TRUE;\
						_stscanf(szValue.Mid(nPos + 1, 2), _T("%X"), &a);\
						_stscanf(szValue.Mid(nPos + 3, 2), _T("%X"), &r);\
						_stscanf(szValue.Mid(nPos + 5, 2), _T("%X"), &g);\
						_stscanf(szValue.Mid(nPos + 7, 2), _T("%X"), &b);\
					}\
					else\
					{\
						_stscanf(szValue.Mid(nPos + 1, 2), _T("%X"), &r);\
						_stscanf(szValue.Mid(nPos + 3, 2), _T("%X"), &g);\
						_stscanf(szValue.Mid(nPos + 5, 2), _T("%X"), &b);\
					}\
					const UIBitmap* pBkBitmap = UIApplication::Instance()->GetAssetManager()->GetRGBImage(RGB(r, g, b), bTrans, a);\
					if(pBkBitmap)\
						obj_var_name.SetBitmap(pBkBitmap);\
				}\
			}\
		}\
		else\
		{\
			int nStart = szValue.Find(_T("source("));\
			int nEnd = szValue.ReverseFind(_T(')'));\
			UIString szImageName;\
			if(nStart != -1)\
				szImageName = szValue.Left(nStart);\
			else\
				szImageName = szValue;\
			szImageName.Trim();\
			const UIBitmap* pBkBitmap = UIApplication::Instance()->GetAssetManager()->GetImage(szImageName);\
			if(!pBkBitmap)\
				return;\
			obj_var_name.SetBitmap(pBkBitmap);\
			if(nStart != -1 && nEnd != -1)\
			{\
				int nIndex = nStart + sizeof(_T("source(")) / sizeof(TCHAR) - 1;\
				UIString szRect = szValue.Mid(nIndex, nEnd - nIndex);\
				szRect.Trim();\
				UIRect rcPos;\
				int nPos = 0;\
				UIString szTemp = szRect.Tokenize(_T(","), nPos);\
				if(!szTemp.IsEmpty())\
					rcPos.left = _ttoi(szTemp);\
				szTemp = szRect.Tokenize(_T(","), nPos);\
				if(!szTemp.IsEmpty())\
					rcPos.top = _ttoi(szTemp);\
				szTemp = szRect.Tokenize(_T(","), nPos);\
				if(!szTemp.IsEmpty())\
					rcPos.right = _ttoi(szTemp);\
				szTemp = szRect.Tokenize(_T(","), nPos);\
				if(!szTemp.IsEmpty())\
					rcPos.bottom = _ttoi(szTemp);\
				obj_var_name.SetPos(rcPos);\
			}\
		}\
	}

#define SET_RECT(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		int nStart = 0;\
		UIString szTemp = szAttrValue.Tokenize(_T(","), nStart);\
		if(!szTemp.IsEmpty())\
			obj_var_name.left = _ttoi(szTemp);\
		szTemp = szAttrValue.Tokenize(_T(","), nStart);\
		if(!szTemp.IsEmpty())\
			obj_var_name.top = _ttoi(szTemp);\
		szTemp = szAttrValue.Tokenize(_T(","), nStart);\
		if(!szTemp.IsEmpty())\
			obj_var_name.right = _ttoi(szTemp);\
		szTemp = szAttrValue.Tokenize(_T(","), nStart);\
		if(!szTemp.IsEmpty())\
			obj_var_name.bottom = _ttoi(szTemp);\
	}

#define SET_SIZE(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		int nPos = 0;\
		UIString szTemp = szAttrValue.Tokenize(_T(","), nPos);\
		if(!szTemp.IsEmpty())\
			obj_var_name.cx = _ttoi(szTemp);\
		szTemp = szAttrValue.Tokenize(_T(","), nPos);\
		if(!szTemp.IsEmpty())\
			obj_var_name.cy = _ttoi(szTemp);\
	}

#define SET_POINT(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		int nStart = 0;\
		UIString szTemp = szAttrValue.Tokenize(_T(","), nStart);\
		if(!szTemp.IsEmpty())\
			obj_var_name.x = _ttoi(szTemp);\
		szTemp = szAttrValue.Tokenize(_T(","), nStart);\
		if(!szTemp.IsEmpty())\
			obj_var_name.y = _ttoi(szTemp);\
	}

#define SET_BOOL(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		if(szAttrValue == _T("true"))\
			obj_var_name = TRUE;\
		else\
			obj_var_name = FALSE;\
	}

#define SET_INT(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		obj_var_name = _ttoi(szAttrValue);\
	}

#define SET_LONG(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		obj_var_name = _ttol(szAttrValue);\
	}

#define SET_DOUBLE(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		obj_var_name = _ttof(szAttrValue);\
	}

#define SET_COLOR(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		if(szAttrValue.GetLength() >= (sizeof(_T("#FFFFFF"))/sizeof(TCHAR) - 1))\
		{\
			int nPos = szAttrValue.Find(_T("#"));\
			if(nPos != -1)\
			{\
				UIString szTemp = szAttrValue;\
				szTemp.MakeUpper();\
				int r,g,b;\
				_stscanf(szTemp.Mid(nPos + 1, 2), _T("%X"), &r);\
				_stscanf(szTemp.Mid(nPos + 3, 2), _T("%X"), &g);\
				_stscanf(szTemp.Mid(nPos + 5, 2), _T("%X"), &b);\
				obj_var_name = RGB(r, g, b);\
			}\
		}\
	}

#define SET_ALIGN(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		int nPos = 0;\
		UIString szTemp = szAttrValue.Tokenize(_T("|"), nPos);\
		if(!szTemp.IsEmpty())\
		{\
			szTemp.MakeUpper();\
			if(szTemp == _T("LEFT"))\
				obj_var_name.cx = UIAlignment::ALIGN_LEFT;\
			else if(szTemp == _T("HCENTER"))\
				obj_var_name.cx = UIAlignment::ALIGN_HCENTER;\
			else if(szTemp == _T("RIGHT"))\
				obj_var_name.cx = UIAlignment::ALIGN_RIGHT;\
			szTemp = szAttrValue.Tokenize(_T(","), nPos);\
			if(!szTemp.IsEmpty())\
			{\
				szTemp.MakeUpper();\
				if(szTemp == _T("TOP"))\
					obj_var_name.cy = UIAlignment::ALIGN_TOP;\
				else if(szTemp == _T("VCENTER"))\
					obj_var_name.cy = UIAlignment::ALIGN_VCENTER;\
				else if(szTemp == _T("BOTTOM"))\
					obj_var_name.cy = UIAlignment::ALIGN_BOTTOM;\
			}\
		}\
		else\
		{\
			if(!szTemp.IsEmpty())\
			{\
				szTemp.MakeUpper();\
				if(szTemp == _T("LEFT"))\
					obj_var_name.cx = UIAlignment::ALIGN_LEFT;\
				else if(szTemp == _T("HCENTER"))\
					obj_var_name.cx = UIAlignment::ALIGN_HCENTER;\
				else if(szTemp == _T("RIGHT"))\
					obj_var_name.cx = UIAlignment::ALIGN_RIGHT;\
			}\
		}\
	}

#define SET_FONT(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		UIString szFontFace;\
		int nFontSize = 0;\
		int nFontStyle = UIFont::NORMAL;\
		int nPos = 0;\
		UIString szTemp = szAttrValue.Tokenize(_T(","), nPos);\
		if(!szTemp.IsEmpty())\
		{\
			szTemp.Trim();\
			szFontFace = szTemp;\
		}\
		szTemp = szAttrValue.Tokenize(_T(","), nPos);\
		if(!szTemp.IsEmpty())\
		{\
			szTemp.Trim();\
			nFontSize = _ttoi(szTemp);\
		}\
		szTemp = szAttrValue.Tokenize(_T(","), nPos);\
		if(!szTemp.IsEmpty())\
		{\
			szTemp.Trim();\
			szTemp.MakeUpper();\
			int nPos = 0;\
			UIString szStyle = szTemp.Tokenize(_T("|"), nPos);\
			while(!szStyle.IsEmpty())\
			{\
				szStyle.Trim();\
				szStyle.MakeUpper();\
				if(szStyle == _T("NORMAL"))\
					nFontStyle |= UIFont::NORMAL;\
				else if(szStyle == _T("BOLD"))\
					nFontStyle |= UIFont::BOLD;\
				else if(szStyle == _T("ITALIC"))\
					nFontStyle |= UIFont::ITALIC;\
				else if(szStyle == _T("UNDERLINED"))\
					nFontStyle |= UIFont::UNDERLINED;\
				szStyle = szTemp.Tokenize(_T("|"), nPos);\
			}\
		}\
		const UIFont* pFont = UIApplication::Instance()->GetAssetManager()->GetFont(szFontFace, nFontSize, nFontStyle);\
		if(obj_var_name)\
			obj_var_name->Release();\
		obj_var_name = pFont;\
	}

#define SET_DIRECTION(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		if(szAttrValue == _T("horizontal"))\
			obj_var_name = Direction::HORIZONTAL;\
		else\
			obj_var_name = Direction::VERTICAL;\
	}

#define SET_SKIN(xml_attr_name, obj_var_name) \
	else if(szAttrName == _T(#xml_attr_name))\
	{\
		obj_var_name->SetSkin(szAttrValue);\
	}

#endif //! __MACROS_H_INCLUDED__