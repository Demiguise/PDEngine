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
	mRenderer = new D3DRenderer(hInstance, mGameWindow->GetMainWHandle(), mClientHeight, mClientWidth);
	mFileManager = new FileManager;
	mEventManager = new EventManager;
	mSceneManager = new SceneManager(mFileManager, mRenderer);
	Camera mCamera = Camera(XMFLOAT3(100.0f, 100.0f, -100.0f));
	mSceneManager->SetActiveCamera(mCamera);
	
	mSceneManager->CreateEntity("Box0", "Models/Box.obj");
	mSceneManager->CreateEntity("Box1", "Models/Box.obj");
	mSceneManager->availableEntities[0].position = XMFLOAT3(1.5f, 0.0f, 1.5f);
	//mSceneManager->CreateEntity("Nyx0", "Models/Nyx.obj");
	//mSceneManager->CreateEntity("Nyx1", "Models/Nyx.obj");
	//mSceneManager->CreateEntity("Nyx2", "Models/Nyx.obj");
}

GameApp::~GameApp(void)
{
	
}

void GameApp::OnWindowChange(UINT msg, WPARAM wParam, LPARAM lParam)
{

}

void GameApp::OnMouseDown(WPARAM btnState, int x, int y)
{

}
void GameApp::OnMouseUp(WPARAM btnState, int x, int y)
{

}
void GameApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	
}

int GameApp::Run(void)
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
			XMMATRIX camViewMatrix = mSceneManager->activeCamera.GetViewMatrix();
			mRenderer->UpdateScene(&camViewMatrix);
			mRenderer->DrawScene();
		}
	}
	return (int)msg.wParam;
}