#include "stdafx.h"
#include "App.h"


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MyApp app;
	app.SetResZip(TEXT("skin.zip"));

	MainWindow mainWnd;
	mainWnd.Create();
	UIRect rc(0, 0, 1024, 800);
	mainWnd.SetBounds(rc);
	mainWnd.CenterWindow();
	mainWnd.SetFrameVisible(TRUE);
	mainWnd.Show();
	return app.Run();
}

