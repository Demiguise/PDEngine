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
	mGameWindow = new GameWindow(this, hInstance, mClientHeight, mClientWidth);
	mRenderer = new D3DRenderer(hInstance, mGameWindow->GetMainWHandle(),
											mClientHeight, mClientWidth);
	mFileManager = new FileManager;
	mEventManager = IEventManager::GetInstance();
	mSceneManager = new SceneManager(mFileManager);
	mCamera = new Camera(mSceneManager->GenerateUID(),
					EnVector3(100.0f, 100.0f, -100.0f));
	mPhysicsManager = new PhysicsManager();
	mSceneManager->RegisterEntity(mCamera);
	mSceneManager->SetActiveCamera(mCamera);
	CreateTestObjects();
	
	//Firing test event
	IEvent* newE = new IEvent();
	newE->eType = "TestEvent";
	int* f = new int(4);
	newE->eData = f;
	mEventManager->QueueEvent(newE);
}

void GameApp::CreateTestObjects()
{
	CRenderableObject* nObj = new CRenderableObject(mSceneManager->GenerateUID(),
									mFileManager->LoadModelData("Models/Box.obj"),
													EnVector3(0.0f, 0.0f, 0.0f));
	mRenderer->CreateBuffer(nObj);
	mSceneManager->RegisterEntity(nObj);
}

GameApp::~GameApp()
{
	delete mCamera;
	mSceneManager->RemoveEntity(mCamera);
}

int GameApp::Run()
{
	MSG msg = {0};

	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mPhysicsManager->Update();
			mEventManager->Update();
			XMMATRIX camViewMatrix = mSceneManager->activeCamera->GetViewMatrix();
			mRenderer->UpdateScene(&camViewMatrix);
			mRenderer->DrawScene();
		}
	}
	return (int)msg.wParam;
}