#ifndef __UIOBJECT_H_INCLUDED__
#define __UIOBJECT_H_INCLUDED__

#include "Macros.h"

class XSKIN_API UIObject
{
public:
					UIObject();
	virtual			~UIObject();

public:
	LPCSTR			Name() const;
	int				HashCode() const;
	BOOL			operator ==(const UIObject& obj) const;
	void			Trace(LPCTSTR szFmt, ...);

private:
					UIObject(const UIObject&);
	UIObject&		operator=(const UIObject&);
};

#endif //! __UIOBJECT_H_INCLUDED__