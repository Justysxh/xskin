#ifndef __UIIMAGE_VIEW_H_INCLUDED__
#define __UIIMAGE_VIEW_H_INCLUDED__

#include "UIView.h"

class UIImageView : public UIView
{
	DECLARE_XML_NODE(UIImageView)
	DECLARE_XML_PROPERTY()


public:
	enum ScaleType
	{
		CENTER = 0,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		SCALE_FILL,
		SCALE_FIT
	};
			UIImageView(UIView* parent = NULL);
			~UIImageView();

	void	SetSource(const UIBitmap* pBitmap, const UIRect& rcPos);
	void	SetSource(const UIString& szFileName, const UIRect& rcPos = UIRect());
	void	SetScaleType(ScaleType st);
	UISize	GetPreferredSize() const OVERRIDE;

protected:
	BOOL	OnCustomAttribute(const UIString& szAttrName, const UIString& szAttrValue) OVERRIDE;
	void	OnPaint(HDC hdc) OVERRIDE;

private:
	UIDrawable m_src;
	ScaleType m_scaleType;
};
#endif //! __UIIMAGE_VIEW_H_INCLUDED__