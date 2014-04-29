#include "GameWindow.h"

GameWindow::GameWindow(GameApp* mGApp, HINSTANCE hInstance, UINT height, UINT width)
{
	mHInstance = hInstance;
	wHeight = height;
	wWidth = width;
	Init();
}

GameWindow::~GameWindow()
{
	
}

LRESULT GameWindow::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			mWMinimized = true;
			mWMaximized = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			mWMaximized = true;
			mWMinimized = false;
		}
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		mInput->Update(msg, wParam, lParam);
		break;
	}
	return DefWindowProc(mHWnd, msg, wParam, lParam);
}

LRESULT CALLBACK GameWindow::StaticMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GameWindow* pThisWindow;

	if( msg == WM_NCCREATE )
	{
		pThisWindow = (GameWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)pThisWindow);
	}
	else
	{
		pThisWindow = (GameWindow*)GetWindowLongPtr(hWnd, GWL_USERDATA);
		if (!pThisWindow)
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	pThisWindow->mHWnd = hWnd;
	return pThisWindow->MsgProc(msg, wParam, lParam);
}

void GameWindow::Init()
{
	GameLog::Log("[GameWindow] Beginning initialisation.", DebugLevel::Normal);
	//Describing the WNDCLASS I want to use.
	WNDCLASS wc;
	wc.lpszClassName = L"BASEGAMEWINDOW";
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = GameWindow::StaticMsgProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

	if (!RegisterClass(&wc))
	{
		//Registering has failed for some reason.
		MessageBox(0, L"Register the window class has failed.", 0, 0);
		GameLog::Log("[GameWindow] Window class registration failed.", DebugLevel::Normal);
		return;
	}

	RECT r = {0, 0, wWidth, wHeight};
	AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
	int adjWidth = r.right - r.left;
	int adjHeight = r.bottom - r.top;


	mHWnd = CreateWindow(	L"BASEGAMEWINDOW",
							L"DirectX11 Framework",
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							adjWidth,
							adjHeight,
							0,
							0,
							mHInstance,
							this);

	DWORD lastError = GetLastError();

	if (!mHWnd)
	{
		//Creating the window has failed for some reason.
		GameLog::Log("[GameWindow] Window creation failed.", DebugLevel::Normal);
		MessageBox(0, L"Creating the window has failed.", 0, 0);
		return;
	}

	mInput = CInput::GetInstance();
	ShowWindow(mHWnd, SW_SHOW);
	UpdateWindow(mHWnd);
	GameLog::Log("[GameWindow] Initialisation Complete.", DebugLevel::Normal);
}

HWND GameWindow::GetMainWHandle()
{
	return mHWnd;
}

HINSTANCE GameWindow::GetMainHInstance()
{
	return mHInstance;
}