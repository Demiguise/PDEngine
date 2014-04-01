#include "D3DRenderer.h"


//Initialisations
D3DRenderer::D3DRenderer(HINSTANCE hInstance, HWND hWnd, UINT wHeight, UINT wWidth)
	:	mWindowHeight(wHeight),
		mWindowWidth (wWidth),
		mDepthStencilBuffer(0),
		mDepthStencilView(0),
		mRenderTargetView(0),
		mSwapChain(0),
		sceneObjData(0)

{
	rendererName = "DirectX11";
	mHInst = hInstance;
	mHWnd = hWnd;	
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
	if (!InitDirect3D()) { return false; }
	InitEffects();
	if (!InitBuffers()) { return false; }
	CreateInputLayer();
	mBufferManager = new D3DBufferManager();
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
		MessageBox(mHWnd, L"D3DDevice creation has failed.", 0, 0);
		return false;
	}
	OnResize(mWindowHeight, mWindowWidth);
	return true;
}

bool D3DRenderer::InitBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = sizeof(Vertex) * 400; // This seems silly and not really "Dynamic"
	vbd.StructureByteStride = 0;
	vbd.MiscFlags = 0;

	md3dDevice->CreateBuffer(&vbd, NULL, &mVBuffer);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(UINT) * 400;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	md3dDevice->CreateBuffer(&ibd, NULL, &mIBuffer);

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

	mProjMatrix = XMMatrixPerspectiveFovLH(0.25f*Pi, newWidth/newHeight, 1.0f, 1000.0f);
}

void D3DRenderer::UpdateScene(std::vector<Entity> activeEntities, XMMATRIX* viewMatrix, bool refreshBuffers)
{
	mCamViewMatrix = *viewMatrix;
	if (refreshBuffers)
	{
		//We make the new concatanated versions of the buffers.
		std::vector<Vertex> sceneVertices;
		std::vector<UINT> sceneIndicies;
		ConCatBufferData(activeEntities, &sceneVertices, &sceneIndicies);
		//Then we remap the data to the buffers.
		D3D11_MAPPED_SUBRESOURCE vResource;
		ZeroMemory(&vResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HR(mDeviceContext->Map(mVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vResource));
		D3D11_MAPPED_SUBRESOURCE iResource;
		ZeroMemory(&iResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HR(mDeviceContext->Map(mIBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &iResource));
		//Next we have the new data applied to the new resources
		memcpy(vResource.pData, &sceneVertices[0], sizeof(Vertex) * sceneVertices.size());
		memcpy(iResource.pData, &sceneIndicies[0], sizeof(UINT) * sceneIndicies.size());
		//Finally we unmap each buffer
		mDeviceContext->Unmap(mVBuffer, 0);
		mDeviceContext->Unmap(mIBuffer, 0);
	}
}

void D3DRenderer::DrawScene()
{
	float bgColor[4] = {0.69f, 0.77f, 0.87f, 1.0f};
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bgColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVBuffer, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	ID3DX11EffectMatrixVariable* mEffectWorldViewProj = mEffect->GetVariableByName("gWorldViewProj")->AsMatrix();

	D3DX11_TECHNIQUE_DESC techDesc;
	mETech->GetDesc(&techDesc);
	for(UINT p = 0 ; p < techDesc.Passes ; ++p)
	{
		for (UINT i = 0 ; i < sceneObjData.size() ; i++)
		{
			XMMATRIX wvpMatrix = BuildWVPMatrix(sceneObjData[i].position, &mCamViewMatrix, &mProjMatrix);
			mEffectWorldViewProj->SetMatrix(reinterpret_cast<float*>(&wvpMatrix));
			mETech->GetPassByIndex(p)->Apply(0, mDeviceContext);
			mDeviceContext->DrawIndexed(sceneObjData[i].iCount,
				sceneObjData[i].startIndLoc, sceneObjData[i].baseVertLoc);
		}
	}


	mSwapChain->Present(0,0);
}

void D3DRenderer::ConCatBufferData(std::vector<Entity> activeEntities, 
				std::vector<Vertex>* oVBData, std::vector<UINT>* oIBData)
{
	sceneObjData.clear();
	std::vector<Vertex>::iterator vIT;
	std::vector<UINT>::iterator iIT;
	for (UINT i = 0 ; i < activeEntities.size() ; i++)
	{
		//Adding to scene render data
		ObjRenderData newObj;
		newObj.iCount = activeEntities[i].mesh.iData.size();
		newObj.startIndLoc = oIBData->size();
		newObj.baseVertLoc = oVBData->size();
		newObj.position = activeEntities[i].position;
		sceneObjData.push_back(newObj);

		//Actual concatenation of buffer data
		vIT = oVBData->end();
		iIT = oIBData->end();
		oVBData->insert(vIT, activeEntities[i].mesh.vData.begin(), activeEntities[i].mesh.vData.end());
		oIBData->insert(iIT, activeEntities[i].mesh.iData.begin(), activeEntities[i].mesh.iData.end());
	}

}

XMMATRIX D3DRenderer::BuildWVPMatrix(XMFLOAT3 wPos, XMMATRIX* view, XMMATRIX* proj)
{
	XMMATRIX worldMatrix = XMMatrixTranslation(wPos.x, wPos.y, wPos.z);
	XMMATRIX lView = *view;
	XMMATRIX lProj = *proj;
	XMMATRIX newMatrix = worldMatrix * lView * lProj;
	return newMatrix;
}

void D3DRenderer::CreateBuffer(ModelData newModel)
{
	
}