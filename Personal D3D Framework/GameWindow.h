#pragma once
#include <Windows.h>
#include <windowsx.h>
#include "GameApp.h"

class GameApp;

class GameWindow
{
public:
	GameWindow(GameApp* mGApp,HINSTANCE hInstance, UINT height, UINT width);
	~GameWindow();
	HWND GetMainWHandle();
	HINSTANCE GetMainHInstance();
	UINT GetCurrentWindowState();

	UINT wHeight;
	UINT wWidth;

protected:
	HWND mHWnd;
	HINSTANCE mHInstance;

private:
	void Init();
	LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	GameApp* mGApp;
	bool mWMinimized;
	bool mWMaximized;
	bool mWResizing;
};