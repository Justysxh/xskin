#ifndef __UILIST_ITEM_H_INCLUDED__
#define __UILIST_ITEM_H_INCLUDED__

#include "UIButton.h"

class XSKIN_API UIListItem : public UIButton
{
	DECLARE_XML_NODE(UIListItem)
	DECLARE_XML_PROPERTY()
public:
			UIListItem(UIView* parent = NULL);
			~UIListItem();

protected:
	void	OnPaint(HDC hdc) OVERRIDE;
	BOOL	OnMousePressed(UIMouseEvent* event) OVERRIDE;
	BOOL	OnMouseClicked(UIMouseEvent* event) OVERRIDE;
};
#endif //! __UILIST_ITEM_H_INCLUDED__