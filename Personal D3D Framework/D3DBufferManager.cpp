#include "D3DBufferManager.h"


D3DBufferManager::D3DBufferManager(void)
{
}


D3DBufferManager::~D3DBufferManager(void)
{
}

void D3DBufferManager::InitNewBuffer(ID3D11Device* md3dDevice, ModelData newModelData)
{
	bufferData newBuffers;
	//Create new Vertex Buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.CPUAccessFlags = 0;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = sizeof(Vertex) * newModelData.vData.size();
	vbd.StructureByteStride = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {0};
	vbData.pSysMem = &newModelData.vData;
	md3dDevice->CreateBuffer(&vbd, &vbData, &newBuffers.vertexBuffer);

	//Create new Index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.CPUAccessFlags = 0;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(UINT) * newModelData.iData.size();
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {0};
	vbData.pSysMem = &newModelData.iData;
	md3dDevice->CreateBuffer(&ibd, &ibData, &newBuffers.indexBuffer);
	
	bufferMap.insert(std::pair<LPCSTR, bufferData>(newModelData.semanticName, newBuffers));
}