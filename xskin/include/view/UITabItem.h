#ifndef __UITAB_ITEM_H_INCLUDED__
#define __UITAB_ITEM_H_INCLUDED__

#include "UIListItem.h"

class UITabItem : public UIListItem
{
	DECLARE_XML_NODE(UITabItem)
	DECLARE_XML_PROPERTY()
public:
					UITabItem(UIView* parent = NULL);
					~UITabItem();
};
#endif //! __UITAB_ITEM_H_INCLUDED__