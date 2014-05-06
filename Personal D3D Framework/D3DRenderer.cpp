#include "D3DRenderer.h"

//Initialisations
D3DRenderer::D3DRenderer(HINSTANCE hInstance, HWND hWnd, UINT wHeight, UINT wWidth)
	:	mWindowHeight(wHeight),
		mWindowWidth (wWidth),
		mDepthStencilBuffer(0),
		mDepthStencilView(0),
		mRenderTargetView(0),
		mSwapChain(0)
{
	rendererName = "DirectX11";
	mHInst = hInstance;
	mHWnd = hWnd;	
	setWireframe = false;
	XMMATRIX i = XMMatrixIdentity(); //Quickly initializing the Projection and View matrices.
	mProjMatrix = mCamViewMatrix = i;
	Init();
}

D3DRenderer::~D3DRenderer()
{
	ReleaseCOM(md3dDevice);
	ReleaseCOM(mDeviceContext);
	ReleaseCOM(mSwapChain);
}

bool D3DRenderer::Init()
{
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[D3DRenderer] Beginning Initialisation.");
	GameLog::GetInstance()->Log(DebugChannel::Rendering, DebugLevel::None, "[D3DRenderer] Testing Rendering Channel! %i", 2);
	if (!InitDirect3D()) { return false; }
	InitEffects();
	CreateInputLayer();
	mBufferManager = new D3DBufferManager(md3dDevice);
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[D3DRenderer] Initialisation Complete.");
	return true;
}

bool D3DRenderer::InitDirect3D()
{
	//Description for the swap chain we're using
	DXGI_SWAP_CHAIN_DESC sd;

	//Defining the swap chain buffer
	sd.BufferDesc.Width = mWindowWidth;
	sd.BufferDesc.Height = mWindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//MSAA Options (Set to no MSAA)
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	
	//Other Options
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mHWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel;
	HR(D3D11CreateDeviceAndSwapChain(	0,
										D3D_DRIVER_TYPE_HARDWARE,
										0,
										D3D11_CREATE_DEVICE_DEBUG,
										0,
										0,
										D3D11_SDK_VERSION,
										&sd,
										&mSwapChain,
										&md3dDevice,
										&featureLevel,
										&mDeviceContext));

	if (!md3dDevice)
	{
		GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal,"[D3DRenderer] D3DDevice creation failed.");
		MessageBox(mHWnd, L"D3DDevice creation has failed.", 0, 0);
		return false;
	}
	OnResize(mWindowHeight, mWindowWidth);
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal,"[D3DRenderer] D3DDevice creation succeeded.");
	return true;
}

void D3DRenderer::CreateInputLayer()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"Position",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC passDesc;
	mETech->GetPassByIndex(0)->GetDesc(&passDesc);
	md3dDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout);

	D3D11_RASTERIZER_DESC rSD;
	rSD.FillMode = D3D11_FILL_WIREFRAME;
	rSD.CullMode = D3D11_CULL_NONE;
	rSD.FrontCounterClockwise = FALSE;
	rSD.DepthBias = 0;
	rSD.SlopeScaledDepthBias = 0.0f;
	rSD.DepthBiasClamp = 0.0f;
	rSD.DepthClipEnable = TRUE;
	rSD.ScissorEnable = FALSE;
	rSD.MultisampleEnable = FALSE;
	rSD.AntialiasedLineEnable = FALSE;

	md3dDevice->CreateRasterizerState(&rSD, &mRSD);
}

void D3DRenderer::InitEffects()
{
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"Effects/color.fx", 0, 0, 0, "fx_5_0", 0, 0, 0, &compiledShader, &compilationMsgs, 0);

	if (compilationMsgs !=0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0 ,0);
		ReleaseCOM(compilationMsgs);
	}

	if (FAILED(hr))
	{
		DXTrace(__FILE__,(DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, md3dDevice, &mEffect));

	ReleaseCOM(compiledShader);
	mETech = mEffect->GetTechniqueByName("ColorTech");
}

//Runtime

void D3DRenderer::OnResize(UINT newHeight, UINT newWidth)
{
	mWindowHeight = newHeight;
	mWindowWidth = newWidth;

	//Release old COM interfaces.
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mRenderTargetView);

	//Resize the buffers to the new sizes and rebind them to the pipeline through
	//the RenderTargetView.
	HR(mSwapChain->ResizeBuffers(1, mWindowWidth, mWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	//Description for the Depth Stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	//Defining the buffer
	depthStencilDesc.Height = mWindowHeight;
	depthStencilDesc.Width = mWindowWidth;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//The MSAA settings for the depthStencilDesc MUST match the SwapChain.
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	//Other options
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	
	//Actually create the DepthStencilView
	md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	//Bind the Render target and DepthStencilView to the pipline
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mScreenViewport.TopLeftX = 0.0f;
	mScreenViewport.TopLeftY = 0.0f;
	mScreenViewport.Width = static_cast<float>(newWidth);
	mScreenViewport.Height = static_cast<float>(newHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;
	
	mDeviceContext->RSSetViewports(1, &mScreenViewport);
	
	mProjMatrix = XMMatrixPerspectiveFovLH(0.25f*3.14f, newWidth/newHeight, 1.0f, 1000.0f);
}

void D3DRenderer::UpdateScene(const EnVector3 &camPos)
{
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR camPosVector = XMVectorSet(camPos.x, camPos.y, camPos.z, 1.0f);
	mCamViewMatrix = XMMatrixLookAtLH(camPosVector, XMVectorZero(), up);
}

void D3DRenderer::DrawScene()
{
	std::vector<packedBufferData> sceneData = mBufferManager->GrabSceneBuffers();

	float bgColor[4] = {0.69f, 0.77f, 0.87f, 1.0f};
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bgColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	if (setWireframe) { mDeviceContext->RSSetState(mRSD); }

	ID3DX11EffectMatrixVariable* mEffectWorldViewProj = mEffect->GetVariableByName("gWorldViewProj")->AsMatrix();

	D3DX11_TECHNIQUE_DESC techDesc;
	mETech->GetDesc(&techDesc);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for(UINT p = 0 ; p < techDesc.Passes ; ++p)
	{
		for (UINT i = 0 ; i < sceneData.size() ; ++i)
		{
			mDeviceContext->IASetVertexBuffers(0, 1, &sceneData[i].bufferContents.vertexBuffer, &stride, &offset);
			mDeviceContext->IASetIndexBuffer(sceneData[i].bufferContents.indexBuffer , DXGI_FORMAT_R32_UINT, 0);
			XMMATRIX wvpMatrix = BuildWVPMatrix(sceneData[i].owningEntity->position, &mCamViewMatrix, &mProjMatrix);
			mEffectWorldViewProj->SetMatrix(reinterpret_cast<float*>(&wvpMatrix));
			mETech->GetPassByIndex(p)->Apply(0, mDeviceContext);
			mDeviceContext->DrawIndexed(sceneData[i].bufferContents.iCount, 0, 0);
		}
	}

	
	mSwapChain->Present(0,0);
}

void D3DRenderer::CreateBuffer(CRenderableObject* newEnt)
{
	mBufferManager->InitNewBuffer(newEnt);
}

void D3DRenderer::DestroyBuffer(CRenderableObject* entity)
{

}

XMMATRIX D3DRenderer::BuildWVPMatrix(EnVector3 wPos, XMMATRIX* view, XMMATRIX* proj)
{
	XMMATRIX worldMatrix = XMMatrixTranslation(wPos.x, wPos.y, wPos.z);
	XMMATRIX lView = *view;
	XMMATRIX lProj = *proj;
	XMMATRIX newMatrix = worldMatrix * lView * lProj;
	return newMatrix;
}
