#ifndef __APP_H_INCLUDED__
#define __APP_H_INCLUDED__

#include "include/XSkin.h"

class ListAdapter : public UIModel
{
public:
					ListAdapter();
					~ListAdapter();

public:
	void			SetData(const std::vector<UIString>& data);
	UIView*			GetView(UIView* pView, int nPos) const OVERRIDE;
	int				GetCount() const OVERRIDE;
	const void*		GetItem(int nPos) const OVERRIDE;
	BOOL			IsEmpty() const OVERRIDE;

protected:
	std::vector<UIString> m_data;
};

// app
class MyApp : public UIApplication
{
public:
			MyApp();
			~MyApp();

protected:
	BOOL	Event(UIEvent* event) OVERRIDE;
};

// main window.
class MainWindow : public UIWindow
{
public:
			MainWindow(UIWindow* parent = NULL);
			~MainWindow();

public:
	BOOL	Event(UIEvent* event) OVERRIDE;
	void	OnCreate() OVERRIDE;
	void	OnDestroy() OVERRIDE;

protected:
	UICardLayout* m_pPager;
};

class Dialog : public UIWindow
{
public:
	Dialog(UIWindow* parent = NULL);
	~Dialog();

public:
	BOOL	Event(UIEvent* event) OVERRIDE;
	void	OnCreate() OVERRIDE;
	void	OnDestroy() OVERRIDE;

private:
	UIButton* m_pClose;
};
#endif //! __APP_H_INCLUDED__