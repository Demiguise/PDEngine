#pragma once
#include "Renderer.h"
#include "D3DBufferManager.h"
#include "D3DCommon.h"
#include "Entity.h"
#include <D3DX11.h>
#include <d3dx11effect.h>
#include <Windows.h>
#include <DxErr.h>

class D3DRenderer : public Renderer
{
public: //Functions
	D3DRenderer (HINSTANCE hInstance, HWND hWnd, UINT wHeight, UINT wWidth);
	~D3DRenderer();
	bool Init();
	void UpdateScene(XMMATRIX* viewMatrix);
	void DrawScene();
	void OnResize(UINT newHeight, UINT newWidth);
	void CreateBuffer(CRenderableObject* newEnt);
	void DestroyBuffer(CRenderableObject* entity);

private:
	bool InitDirect3D();
	bool InitBuffers();
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
	ID3D11RasterizerState* mRSD;

	//Buffers
	D3DBufferManager* mBufferManager;
	ID3D11Buffer* mVBuffer;
	ID3D11Buffer* mIBuffer;

	//Windows handles
	HINSTANCE mHInst;
	HWND mHWnd;

	//Window Variables
	int mWindowHeight;
	int mWindowWidth;

	//Others
	XMMATRIX mCamViewMatrix;
	XMMATRIX mProjMatrix;
};

