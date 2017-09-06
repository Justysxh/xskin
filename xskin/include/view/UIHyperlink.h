#ifndef __UIHYPERLINK_H_INCLUDED__
#define __UIHYPERLINK_H_INCLUDED__

#include "UIButton.h"

class XSKIN_API UIHyperlink : public UIButton
{
	DECLARE_XML_NODE(UIHyperlink)
public:
			UIHyperlink(UIView* parent = NULL);
			~UIHyperlink();
public:
	UISize	GetPreferredSize() const OVERRIDE;

protected:
	void	OnPaint(HDC hdc) OVERRIDE;
};
#endif //! __UIHYPERLINK_H_INCLUDED__