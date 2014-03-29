#pragma once
#include "Common.h"
#include "Entity.h"
#include <D3DX11.h>
#include <d3dx11effect.h>
#include <Windows.h>
#include <DxErr.h>


class D3DRenderer
{
public: //Functions
	D3DRenderer(HINSTANCE hInstance, HWND hWnd, UINT wHeight, UINT wWidth);
	~D3DRenderer();
	bool Init();
	void UpdateScene(std::vector<Entity> activeEntities, XMMATRIX* viewMatrix, bool refreshBuffers);
	void DrawScene();
	void OnResize(UINT newHeight, UINT newWidth);

private:
	bool InitDirect3D();
	bool InitBuffers();
	void ConCatBufferData(std::vector<Entity> activeEntities, 
		std::vector<Vertex>* oVBData, std::vector<UINT>* oIBData);
	void CreateInputLayer();
	void InitEffects();
	XMMATRIX BuildWVPMatrix(XMFLOAT3 wPos, XMMATRIX* view, XMMATRIX* proj);


	//DirectX COM Devices
	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RenderTargetView* mRenderTargetView;
	IDXGISwapChain* mSwapChain;
	ID3D11InputLayout* mInputLayout;
	ID3DX11Effect* mEffect;
	ID3DX11EffectTechnique* mETech;
	D3D11_VIEWPORT mScreenViewport;

	//Buffers
	ID3D11Buffer* mVBuffer;
	ID3D11Buffer* mIBuffer;

	//Windows handles
	HINSTANCE mHInst;
	HWND mHWnd;

	//Window Variables
	int mWindowHeight;
	int mWindowWidth;

	//Others
	std::vector<ObjRenderData> sceneObjData;
	XMMATRIX mCamViewMatrix;
	XMMATRIX mProjMatrix;
};

//Helpful Macros (Shameless 'Stolen' from Frank D Luna)

#define ReleaseCOM(x) { if(x) { x->Release(); x = 0; } }

//Need to have the '\' at the end of the line to say it's a
//line macro. Since they are usually just a single line.
#define HR(x)												\
{															\
	HRESULT hr = x;											\
	if (FAILED(hr))											\
	{														\
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);	\
	}														\
}

#define Pi 3.14f