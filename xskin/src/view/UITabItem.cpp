#include "stdafx.h"
#include "../../include/view/UITabItem.h"

IMPLEMENT_XML_NODE(UITabItem)

BEGIN_XML_PROPERTY(UITabItem)
END_XML_PROPERTY()

UITabItem::UITabItem(UIView* parent)
: UIListItem(parent)
{
	m_alignment.cx = UIAlignment::ALIGN_HCENTER;
	m_alignment.cy = UIAlignment::ALIGN_VCENTER;
}

UITabItem::~UITabItem()
{

}