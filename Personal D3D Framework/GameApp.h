#pragma once
#include "Common.h"
#include "GameWindow.h"
#include "D3DRenderer.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "IEventManager.h"
#include "PhysicsManager.h"
#include "RenderableObject.h" //For test purposes, should probably remove once finished.
#include "GameLog.h"
#include "Timer.h"
#include <Awesomium\WebCore.h>

class GameWindow;

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();
	int Run();
	void CreateTestObjects();

	FileManager* mFileManager;
	GameWindow* mGameWindow;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
	IEventManager* mEventManager;
	PhysicsManager* mPhysicsManager;
	Camera* mCamera;
	Timer* mTimer;
	Timer* physicsTimer;
};

