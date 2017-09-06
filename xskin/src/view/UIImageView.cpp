#include "stdafx.h"
#include "../../include/view/UIImageView.h"

IMPLEMENT_XML_NODE(UIImageView)

BEGIN_XML_PROPERTY(UIImageView)
	SET_DRAWABLE(src, m_src)
END_XML_PROPERTY()

UIImageView::UIImageView(UIView* parent)
 : UIView(parent)
{
	m_scaleType = ScaleType::SCALE_FILL;
}

UIImageView::~UIImageView()
{

}

void UIImageView::SetSource(const UIBitmap* pBitmap, const UIRect& rcPos)
{
	m_src.SetBitmap(pBitmap);
	m_src.SetPos(rcPos);
}

void UIImageView::SetSource(const UIString& szFileName, const UIRect& rcPos)
{
	const UIBitmap* pBitmap = UIApplication::Instance()->GetAssetManager()->GetImage(szFileName);
	if(pBitmap)
	{
		m_src.SetBitmap(pBitmap);
		m_src.SetPos(rcPos);
	}
}

void UIImageView::SetScaleType(ScaleType st)
{
	m_scaleType = st;
}

UISize UIImageView::GetPreferredSize() const
{
	UISize size = __super::GetPreferredSize();

	if(m_src.IsValid())
	{
		size.cx = max(size.cx, m_src.Width());
		size.cy = max(size.cy, m_src.Height());
	}

	return size;
}

BOOL UIImageView::OnCustomAttribute(const UIString& szAttrName, const UIString& szAttrValue)
{
	if (szAttrName == _T("scaleType"))
	{
		if (szAttrValue.CompareNoCase(_T("CENTER")) == 0)
			m_scaleType = ScaleType::CENTER;
		else if (szAttrValue.CompareNoCase(_T("LEFT")) == 0)
			m_scaleType = ScaleType::LEFT;
		else if (szAttrValue.CompareNoCase(_T("RIGHT")) == 0)
			m_scaleType = ScaleType::RIGHT;
		else if (szAttrValue.CompareNoCase(_T("TOP")) == 0)
			m_scaleType = ScaleType::TOP;
		else if (szAttrValue.CompareNoCase(_T("BOTTOM")) == 0)
			m_scaleType = ScaleType::BOTTOM;
		else if (szAttrValue.CompareNoCase(_T("TOP_LEFT")) == 0)
			m_scaleType = ScaleType::TOP_LEFT;
		else if (szAttrValue.CompareNoCase(_T("TOP_RIGHT")) == 0)
			m_scaleType = ScaleType::TOP_RIGHT;
		else if (szAttrValue.CompareNoCase(_T("BOTTOM_LEFT")) == 0)
			m_scaleType = ScaleType::BOTTOM_LEFT;
		else if (szAttrValue.CompareNoCase(_T("BOTTOM_RIGHT")) == 0)
			m_scaleType = ScaleType::BOTTOM_RIGHT;
		else if (szAttrValue.CompareNoCase(_T("SCALE_FILL")) == 0)
			m_scaleType = ScaleType::SCALE_FILL;
		else if (szAttrValue.CompareNoCase(_T("SCALE_FIT")) == 0)
			m_scaleType = ScaleType::SCALE_FIT;

		return TRUE;
	}

	return FALSE;
}

void UIImageView::OnPaint(HDC hdc)
{
	UIRect rcPaint(0, 0, m_rcBounds.Width(), m_rcBounds.Height());
	m_bkgnd.Draw(hdc, rcPaint, UIDrawable::FILL_REPEAT_BOTH, m_nOpacity);

	if(m_scaleType == ScaleType::CENTER)
	{
		rcPaint.left = (m_rcBounds.Width() - m_src.Width()) / 2;
		rcPaint.right = rcPaint.left + m_src.Width();
		rcPaint.top = (m_rcBounds.Height() - m_src.Height()) / 2;
		rcPaint.bottom = rcPaint.top + m_src.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::LEFT)
	{
		rcPaint.left = 0;
		rcPaint.right = m_src.Width();
		rcPaint.top = (m_rcBounds.Height() - m_src.Height()) / 2;
		rcPaint.bottom = rcPaint.top + m_src.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::RIGHT)
	{
		rcPaint.right = m_rcBounds.Width();
		rcPaint.left = rcPaint.right - m_src.Width();
		rcPaint.top = (m_rcBounds.Height() - m_src.Height()) / 2;
		rcPaint.bottom = rcPaint.top + m_src.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::TOP)
	{
		rcPaint.top = 0;
		rcPaint.bottom = m_src.Height();
		rcPaint.left = (m_rcBounds.Width() - m_src.Width()) / 2;
		rcPaint.right = rcPaint.left + m_src.Width();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::BOTTOM)
	{
		rcPaint.bottom = m_rcBounds.Height();
		rcPaint.top = rcPaint.bottom - m_src.Height();
		rcPaint.left = (m_rcBounds.Width() - m_src.Width()) / 2;
		rcPaint.right = rcPaint.left + m_src.Width();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::TOP_LEFT)
	{
		rcPaint.left = 0;
		rcPaint.top = 0;
		rcPaint.right = m_src.Width();
		rcPaint.bottom = m_src.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::TOP_RIGHT)
	{
		rcPaint.right = m_rcBounds.Width();
		rcPaint.top = 0;
		rcPaint.left = rcPaint.right - m_src.Width();
		rcPaint.bottom = m_src.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::BOTTOM_LEFT)
	{
		rcPaint.left = 0;
		rcPaint.bottom = m_rcBounds.Height();
		rcPaint.right = m_src.Width();
		rcPaint.top = rcPaint.bottom - m_src.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::BOTTOM_RIGHT)
	{
		rcPaint.right = m_rcBounds.Width();
		rcPaint.bottom = m_rcBounds.Height();
		rcPaint.left = rcPaint.right - m_src.Width();
		rcPaint.top = rcPaint.bottom - m_src.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_NONE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::SCALE_FILL)
	{
		rcPaint.left = 0;
		rcPaint.top = 0;
		rcPaint.right = m_rcBounds.Width();
		rcPaint.bottom = m_rcBounds.Height();
		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_SCALE, m_nOpacity);
	}
	else if(m_scaleType == ScaleType::SCALE_FIT)
	{
		int nImageWidth = m_src.Width();
		int nImageHeight = m_src.Height();
		int nFrameWidth = m_rcBounds.Width();
		int nFrameHeight = m_rcBounds.Height();
		double ratio;
		int width, height;

		if(nImageWidth > nImageHeight)
		{
			ratio = static_cast<double>(nFrameWidth) / nImageWidth;
			width = static_cast<int>(nImageWidth * ratio);
			height = static_cast<int>(nImageHeight * ratio);
			if(height > nFrameHeight)
				ratio = static_cast<double>(nFrameHeight) / height;
			else
				ratio = 1.0;

			width = static_cast<int>(width * ratio);
			height = static_cast<int>(height * ratio);
		}
		else
		{
			ratio = static_cast<double>(nFrameHeight) / nImageHeight;
			width = static_cast<int>(nImageWidth * ratio);
			height = static_cast<int>(nImageHeight * ratio);

			if(width > nFrameWidth)
				ratio = static_cast<double>(nFrameWidth) / width;
			else
				ratio = 1.0;

			width = static_cast<int>(width * ratio);
			height = static_cast<int>(height * ratio);
		}

		rcPaint.left = (nFrameWidth - width) / 2;
		rcPaint.right = rcPaint.left + width;
		rcPaint.top = (nFrameHeight - height) / 2;
		rcPaint.bottom = rcPaint.top + height;

		m_src.Draw(hdc, rcPaint, UIDrawable::FILL_SCALE, m_nOpacity);
	}
}