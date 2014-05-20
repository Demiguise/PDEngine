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
	mFileManager = FileManager::GetInstance();
	mEventManager = IEventManager::GetInstance();
	mGameWindow = new GameWindow(this, hInstance, mClientHeight, mClientWidth);
	mRenderer = new D3DRenderer(hInstance, mGameWindow->GetMainWHandle(),
											mClientHeight, mClientWidth);
	mSceneManager = new SceneManager(mFileManager);
	mCamera = new Camera(mSceneManager->GenerateUID(),
					EnVector3(00.0f, 0.0f, 30.0f));

	std::vector<ModelData> colliders;
	colliders.push_back(mFileManager->LoadModelData("Models/Colliders/RBCube.obj"));
	colliders.push_back(mFileManager->LoadModelData("Models/Colliders/RBCylinder.obj"));
	colliders.push_back(mFileManager->LoadModelData("Models/Colliders/RBSphere.obj"));
	mPhysicsManager = new PhysicsManager(colliders);
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
													EnVector3(-4.0f, 0.0f, 0.0f));
	mRenderer->CreateBuffer(nObj);
	mSceneManager->RegisterEntity(nObj);
	mPhysicsManager->RegisterEntity(nObj, ColliderType::Box, 1);
	nObj->AddForce(EnVector3(1.0f, 0.0f, 0.0f), 250);
	nObj->name = "Box 1";

	CRenderableObject* nObj2 = new CRenderableObject(mSceneManager->GenerateUID(),
									mFileManager->LoadModelData("Models/Box.obj"),
													EnVector3(4.0f, 0.0f, 0.0f));
	mRenderer->CreateBuffer(nObj2);
	mSceneManager->RegisterEntity(nObj2);
	mPhysicsManager->RegisterEntity(nObj2, ColliderType::Box, 1);
	nObj2->AddForce(EnVector3(-1.0f, 0.0f, 0.0f), 250);
	nObj2->name = "Box 2";
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
			}
			
			mEventManager->Update();
			mSceneManager->UpdateEntities();
			mRenderer->UpdateScene(mSceneManager->activeCamera->position);
			mRenderer->DrawScene();
		}
	}
	return (int)msg.wParam;
}