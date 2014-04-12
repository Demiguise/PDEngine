#pragma once
#include <Windows.h>
#include "GameWindow.h"
#include "D3DRenderer.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "IEventManager.h"

class GameWindow;

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp(void);
	int Run(void);

	void OnWindowChange(UINT msg, WPARAM wParam, LPARAM lParam);
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	FileManager* mFileManager;
	GameWindow* mGameWindow;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
	IEventManager* mEventManager;
};

