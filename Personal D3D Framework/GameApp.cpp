#include "GameApp.h"

#define mClientWidth 800
#define mClientHeight 600

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR cmdLine, int showCmd)
{
	GameApp mGApp(hInstance);
	return mGApp.Run();
}

GameApp::GameApp(HINSTANCE hInstance)
{
	mEventManager = IEventManager::GetInstance();
	mGameWindow = new GameWindow(this, hInstance, mClientHeight, mClientWidth);
	mRenderer = new D3DRenderer(hInstance, mGameWindow->GetMainWHandle(),
											mClientHeight, mClientWidth);
	mFileManager = new FileManager;
	mSceneManager = new SceneManager(mFileManager);
	mCamera = new Camera(mSceneManager->GenerateUID(),
					EnVector3(30.0f, 30.0f, -30.0f));
	mPhysicsManager = new PhysicsManager();
	mSceneManager->RegisterEntity(mCamera);
	mSceneManager->SetActiveCamera(mCamera);
	mTimer = new Timer();
	physicsTimer = new Timer();
	CreateTestObjects();
}

void GameApp::CreateTestObjects()
{
	CRenderableObject* nObj = new CRenderableObject(mSceneManager->GenerateUID(),
									mFileManager->LoadModelData("Models/Box.obj"),
													EnVector3(-2.0f, 0.0f, 0.0f));
	mRenderer->CreateBuffer(nObj);
	mSceneManager->RegisterEntity(nObj);
	mPhysicsManager->RegisterEntity(nObj);
	nObj->SetRigidBody(1, 1, 1.0f);
	nObj->AddForce(EnVector3(1.0f, 0.0f, 0.0f), 250);

	CRenderableObject* nObj2 = new CRenderableObject(mSceneManager->GenerateUID(),
									mFileManager->LoadModelData("Models/Box.obj"),
													EnVector3(2.0f, 0.0f, 0.0f));
	mRenderer->CreateBuffer(nObj2);
	mSceneManager->RegisterEntity(nObj2);
	mPhysicsManager->RegisterEntity(nObj2);
	nObj2->SetRigidBody(1, 1, 1.0f);
	nObj2->AddForce(EnVector3(-1.0f, 0.0f, 0.0f), 250);
}

GameApp::~GameApp()
{
	delete mCamera;
	mSceneManager->RemoveEntity(mCamera);
}

int GameApp::Run()
{
	MSG msg = {0};
	mTimer->startTimer();
	physicsTimer->startTimer();
	while(msg.message != WM_QUIT)
	{
		mTimer->resetTimer();
		if (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (physicsTimer->elapsedTime() > 0.005f)
			{
				mPhysicsManager->Update(0.005f);
				physicsTimer->resetTimer();
				if (mPhysicsManager->RayCast(EnVector3(0.0f,0.0f,0.0f), EnVector3(1.0f, 0.0f, 0.0f)))
				{
					OutputDebugString(L"Raycast is currently intersecting an object!\n");
				}
			}
			
			mEventManager->Update();
			mRenderer->UpdateScene(mSceneManager->activeCamera->position);
			mRenderer->DrawScene();
		}
	}
	return (int)msg.wParam;
}