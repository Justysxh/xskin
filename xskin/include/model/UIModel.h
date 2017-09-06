#ifndef __UIMODEL_H_INCLUDED__
#define __UIMODEL_H_INCLUDED__

#include <vector>
#include "../core/UIObject.h"

class UIView;
class UIString;
class XSKIN_API UIModel : public UIObject
{
public:
							UIModel();
							~UIModel();

public:
	virtual UIView*			GetView(UIView* pView, int nPos) const = 0;
	virtual int				GetCount() const = 0;
	virtual const void*		GetItem(int nPos) const = 0;
	virtual BOOL			IsEmpty() const = 0;

};

class XSKIN_API UIStringModel : public UIModel
{
public:
					UIStringModel();
					~UIStringModel();

public:
	void			Append(const UIString& str);
	UIView*			GetView(UIView* pView, int nPos) const OVERRIDE;
	int				GetCount() const OVERRIDE;
	const void*		GetItem(int nPos) const OVERRIDE;
	BOOL			IsEmpty() const OVERRIDE;

private:
	std::vector<UIString> m_items;
};
#endif //! __UIADAPTER_H_INCLUDED__