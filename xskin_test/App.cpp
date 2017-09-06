#include "stdafx.h"
#include "App.h"


ListAdapter::ListAdapter()
{
	for(int x = 0; x < 2; ++x)
	{
		UIString s;
		s.Format(_T("item-%d"), x);
		m_data.push_back(s);
	}
}

ListAdapter::~ListAdapter()
{

}

void ListAdapter::SetData(const std::vector<UIString>& data)
{
	m_data.clear();
	m_data.insert(m_data.begin(), data.begin(), data.end());
}

UIView* ListAdapter::GetView(UIView* pView, int nPos) const
{
	if(pView == NULL)
	{
		pView = UIView::Inflate(_T("listitem.xml"));
	}
	return pView;
}

int ListAdapter::GetCount() const
{
	return m_data.size();
}

const void* ListAdapter::GetItem(int nPos) const
{
	return m_data[nPos].GetData();
}

BOOL ListAdapter::IsEmpty() const
{
	return FALSE;
}

MyApp::MyApp()
{

}

MyApp::~MyApp()
{

}

BOOL MyApp::Event(UIEvent* event)
{
	return __super::Event(event);
}

MainWindow::MainWindow(UIWindow* parent /* = NULL */)
 : UIWindow(parent)
{
	m_pPager = NULL;
}

MainWindow::~MainWindow()
{

}



BOOL MainWindow::Event(UIEvent* event)
{
	if(event->GetType() == UIEvent::EVENT_ACTION)
	{
		UIActionEvent* action = static_cast<UIActionEvent*>(event);
		UIView* v = static_cast<UIView*>(event->GetSource());
		UIString id = v->GetID();

		XTRACE(_T("id:%s, command:%s"), id, action->GetCommand());

		if(id == _T("tabbar"))
		{
			int nSel = action->GetFlags();
			switch(nSel)
			{
			case 0:	if(m_pPager) m_pPager->Show(_T("page_uibutton")); break;
			case 1:	if(m_pPager) m_pPager->Show(_T("page_uiimagebutton")); break;
			case 2: if(m_pPager) m_pPager->Show(_T("page_uiradiobutton")); break;
			case 3: if(m_pPager) m_pPager->Show(_T("page_uicheckbox")); break;
			case 4: if(m_pPager) m_pPager->Show(_T("page_uiimageview")); break;
			case 5: if(m_pPager) m_pPager->Show(_T("page_uiscrollview")); break;
			case 6: if(m_pPager) m_pPager->Show(_T("page_uiprogressbar")); break;
			case 7: if(m_pPager) m_pPager->Show(_T("page_uislider")); break;
			case 8: if(m_pPager) m_pPager->Show(_T("page_uitextview")); break;
			case 9: if(m_pPager) m_pPager->Show(_T("page_uihyperlink")); break;
			case 10: if(m_pPager) m_pPager->Show(_T("page_uieditview")); break;
			case 11: if(m_pPager) m_pPager->Show(_T("page_uicombobox")); break;
			case 12: if(m_pPager) m_pPager->Show(_T("page_uilistview")); break;
			case 13:
			{
				if(m_pPager) m_pPager->Show(_T("page_uianimation"));

				// translation animation.
				UIView* v = static_cast<UIButton*>(GetRootView()->GetViewForID(_T("btn_translate")));
				if(v)
				{
					if(v->GetAnimation())
					{
						v->GetAnimation()->Stop();
						v->GetAnimation()->Start();
					}
					else
					{
						UITranslateAnimation* pAnim = new UITranslateAnimation(v, -100, 100, 0, 0);
						pAnim->SetDuration(800);
						pAnim->Start();
						pAnim->SetRepeatCount(INFINITE);
						pAnim->SetRepeatMode(UIAnimation::REVERSE);
					}
				}

				// scale.
				v = static_cast<UIButton*>(GetRootView()->GetViewForID(_T("btn_scale")));
				if(v)
				{
					if(v->GetAnimation())
					{
						v->GetAnimation()->Stop();
						v->GetAnimation()->Start();
					}
					else
					{
						UIScaleAnimation* pAnim = new UIScaleAnimation(v, 0.5, 2.0, 0.5, 2.0);
						pAnim->SetDuration(800);
						pAnim->Start();
						pAnim->SetRepeatCount(INFINITE);
						pAnim->SetRepeatMode(UIAnimation::REVERSE);
					}
				}

				// alpha.
				v = static_cast<UIButton*>(GetRootView()->GetViewForID(_T("btn_alpha")));
				if(v)
				{
					if(v->GetAnimation())
					{
						v->GetAnimation()->Stop();
						v->GetAnimation()->Start();
					}
					else
					{
						UIAlphaAnimation* pAnim = new UIAlphaAnimation(v, 0, 255);
						pAnim->SetDuration(800);
						pAnim->Start();
						pAnim->SetRepeatCount(INFINITE);
						pAnim->SetRepeatMode(UIAnimation::REVERSE);
					}
				}

			}
			break;
			case 14: if(m_pPager) m_pPager->Show(_T("page_uigridlayout")); break;
			case 15: if(m_pPager) m_pPager->Show(_T("page_uidialog")); break;
			}

		}
		else if(id == _T("test_dialog"))
		{
			Dialog dlg(v->GetRootView()->GetWindow());
			dlg.Create();
			dlg.SetBounds(0, 0, 320, 240);
			dlg.ShowModal();
		}
	}
	return __super::Event(event);
}

void MainWindow::OnCreate()
{
	SetContentView(_T("demo.xml"));

	m_pPager = dynamic_cast<UICardLayout*>(GetRootView()->GetViewForID(_T("pager")));

	UIComboBox* pCombo = dynamic_cast<UIComboBox*>(GetRootView()->GetViewForID(_T("combo1")));
	if(pCombo)
	{
		UIStringModel* sm = new UIStringModel;
		for(int x = 0; x < 20; ++x)
			sm->Append(_T("combo1 text"));
		pCombo->SetModel(sm);
	}

	pCombo = dynamic_cast<UIComboBox*>(GetRootView()->GetViewForID(_T("combo2")));
	if(pCombo)
	{
		UIStringModel* sm = new UIStringModel;
		for(int x = 0; x < 20; ++x)
			sm->Append(_T("combo2 text"));
		pCombo->SetModel(sm);
	}

	UIListView* pList = dynamic_cast<UIListView*>(GetRootView()->GetViewForID(_T("list1")));
	if(pList)
	{
		UIStringModel* sm = new UIStringModel;
		for(int x = 0; x < 20; ++x)
			sm->Append(_T("list1 text"));
		pList->SetModel(sm);
	}

	pList = dynamic_cast<UIListView*>(GetRootView()->GetViewForID(_T("list2")));
	if(pList)
	{
		UIStringModel* sm = new UIStringModel;
		for(int x = 0; x < 40; ++x)
			sm->Append(_T("list2 text"));
		pList->SetModel(sm);
	}

	pList = dynamic_cast<UIListView*>(GetRootView()->GetViewForID(_T("list3")));
	if(pList)
	{
		UIStringModel* sm = new UIStringModel;
		for(int x = 0; x < 20; ++x)
			sm->Append(_T("disabled"));
		pList->SetModel(sm);
	}


	//v = static_cast<UIImageButton*>(GetRootView()->GetViewForID(_T("btn_bhzz")));
	//if(v)
	//{
	//	UITranslateAnimation* pAnim = new UITranslateAnimation(v, -1000, 0, 0, 0);
	//	pAnim->SetDuration(800);
	//	pAnim->Start();
	//	pAnim->SetRepeatCount(50);
	//	pAnim->SetRepeatMode(UIAnimation::REVERSE);
	//}

	//v = static_cast<UIImageButton*>(GetRootView()->GetViewForID(_T("btn_khzz")));
	//if(v)
	//{
	//	UITranslateAnimation* pAnim = new UITranslateAnimation(v, -1000, 0, 0, 0);
	//	pAnim->SetDuration(700);
	//	pAnim->Start();
	//	pAnim->SetRepeatCount(50);
	//	pAnim->SetRepeatMode(UIAnimation::REVERSE);
	//}

	//v = static_cast<UIImageButton*>(GetRootView()->GetViewForID(_T("btn_sf")));
	//if(v)
	//{
	//	UITranslateAnimation* pAnim = new UITranslateAnimation(v, 1000, 0, 0, 0);
	//	pAnim->SetDuration(800);
	//	pAnim->Start();
	//	pAnim->SetRepeatCount(50);
	//	pAnim->SetRepeatMode(UIAnimation::REVERSE);
	//}

	//v = static_cast<UIImageButton*>(GetRootView()->GetViewForID(_T("btn_df")));
	//if(v)
	//{
	//	UITranslateAnimation* pAnim = new UITranslateAnimation(v, 1000, 0, 0, 0);
	//	pAnim->SetDuration(900);
	//	pAnim->Start();
	//	pAnim->SetRepeatCount(50);
	//	pAnim->SetRepeatMode(UIAnimation::REVERSE);
	//}

	//v = static_cast<UIImageButton*>(GetRootView()->GetViewForID(_T("btn_qf")));
	//if(v)
	//{
	//	UITranslateAnimation* pAnim = new UITranslateAnimation(v, 1000, 0, 0, 0);
	//	pAnim->SetDuration(900);
	//	pAnim->Start();
	//	pAnim->SetRepeatCount(50);
	//	pAnim->SetRepeatMode(UIAnimation::REVERSE);
	//}

	//v = static_cast<UIImageButton*>(GetRootView()->GetViewForID(_T("btn_kh")));
	//if(v)
	//{
	//	UIScaleAnimation* pAnim = new UIScaleAnimation(v, 0.0, 1.0, 0.0, 1.0);
	//	pAnim->SetDuration(1000);
	//	pAnim->SetRepeatCount(200);
	//	pAnim->SetTweenType(UIAnimation::ELASTIC_EASE_OUT);
	//	pAnim->SetRepeatMode(UIAnimation::REVERSE);
	//	pAnim->SetScaleAnchor(UIScaleAnimation::ANCHOR_TOPRIGHT);
	//	pAnim->Start();
	//}

	//v = static_cast<UIImageView*>(GetRootView()->GetViewForID(_T("img")));
	//if(v)
	//{
	//	UITranslateAnimation* pAnim = new UITranslateAnimation(v, 0, 0, 0, 400);
	//	pAnim->SetDuration(1000);
	//	pAnim->SetRepeatCount(200);
	//	pAnim->SetTweenType(UIAnimation::BOUNCE_EASE_OUT);
	//	pAnim->SetRepeatMode(UIAnimation::RESTART);
	//	pAnim->Start();
	//}

	//v = static_cast<UIButton*>(GetRootView()->GetViewForID(_T("enter")));
	//if(v)
	//{
	//	UIAlphaAnimation* pAnim = new UIAlphaAnimation(v, 0, 255);
	//	pAnim->SetDuration(600);
	//	pAnim->SetRepeatCount(200);
	//	pAnim->SetRepeatMode(UIAnimation::REVERSE);
	//	pAnim->Start();
	//}
}

void MainWindow::OnDestroy()
{
	UIApplication::Instance()->Exit(0);
}

Dialog::Dialog(UIWindow* parent /* = NULL */)
 : UIWindow(parent)
{
	m_pClose = NULL;
}

Dialog::~Dialog()
{

}

void Dialog::OnCreate()
{
	SetContentView(_T("dialog.xml"));
	m_pClose = dynamic_cast<UIButton*>(GetRootView()->GetViewForID(_T("btn_close")));
}

void Dialog::OnDestroy()
{
	UIApplication::Instance()->Exit(0);
}

BOOL Dialog::Event(UIEvent* event)
{
	if(event->GetType() == UIEvent::EVENT_ACTION)
	{
		if(event->GetSource() == m_pClose)
		{
			this->Close();
		}
	}

	return __super::Event(event);
}