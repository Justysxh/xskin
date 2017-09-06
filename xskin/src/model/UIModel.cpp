#include "stdafx.h"
#include "../../include/model/UIModel.h"
#include "../../include/core/UIString.h"
#include "../../include/view/UIListItem.h"

UIModel::UIModel()
{

}

UIModel::~UIModel()
{

}

UIStringModel::UIStringModel()
{

}

UIStringModel::~UIStringModel()
{

}

void UIStringModel::Append(const UIString& str)
{
	m_items.push_back(str);
}

UIView* UIStringModel::GetView(UIView* pView, int nPos) const
{
	UIListItem* pItem;
	if(pView)
	{
		pItem = static_cast<UIListItem*>(pView);
		pItem->SetText(m_items[nPos]);
	}
	else
	{
		pItem = new UIListItem;
		pItem->SetText(m_items[nPos]);
		pItem->SetSizePolicy(UISizePolicy::EXPANDING, UISizePolicy::PREFERRED);
		const UIBitmap* pNormalBitmap = UIApplication::Instance()->GetAssetManager()->GetRGBImage(RGB(229, 237, 242));
		const UIBitmap* pCheckedBitmap = UIApplication::Instance()->GetAssetManager()->GetRGBImage(RGB(70, 185, 240));
		pItem->SetBkHot(pNormalBitmap, UIRect(0, 0, 0, 0));
		pItem->SetBkChecked(pCheckedBitmap, UIRect(0, 0, 0, 0));
	}

	return pItem;
}

int UIStringModel::GetCount() const
{
	return m_items.size();
}

const void* UIStringModel::GetItem(int nPos) const
{
	return m_items[nPos].GetData();
}

BOOL UIStringModel::IsEmpty() const
{
	return m_items.size() == 0 ? TRUE : FALSE;
}
