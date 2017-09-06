#ifndef __REFCOUNTED_H_INCLUDED__
#define __REFCOUNTED_H_INCLUDED__

#include "UIObject.h"

class XSKIN_API RefCounted
{
public:
			RefCounted() { m_nRefCount = 0; }
	virtual ~RefCounted() { }

public:
	void	AddRef() const { ++m_nRefCount; }
	void	Release() const { if(--m_nRefCount == 0) delete this; }

private:
	mutable int m_nRefCount;
};

#endif //! __REFCOUNTED_H_INCLUDED__