#include "D3DBufferManager.h"


D3DBufferManager::D3DBufferManager(ID3D11Device* mDevice)
{
	md3dDevice = mDevice;
}


D3DBufferManager::~D3DBufferManager()
{

}

void D3DBufferManager::InitNewBuffer(Entity* newEntity)
{
	bufferData newBuffers;
	//Create new Vertex Buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.CPUAccessFlags = 0;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = sizeof(Vertex) * newEntity->mesh.vData.size();
	vbd.StructureByteStride = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {0};
	vbData.pSysMem = &newEntity->mesh.vData[0];
	md3dDevice->CreateBuffer(&vbd, &vbData, &newBuffers.vertexBuffer);

	//Create new Index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.CPUAccessFlags = 0;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(UINT) * newEntity->mesh.iData.size();
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {0};
	ibData.pSysMem = &newEntity->mesh.iData[0];
	md3dDevice->CreateBuffer(&ibd, &ibData, &newBuffers.indexBuffer);
	
	newBuffers.iCount = newEntity->mesh.iData.size();
	bufferMap.insert(std::pair<Entity*, bufferData>(newEntity, newBuffers));
}

void D3DBufferManager::DestroyBuffer(Entity* entityToDestroy)
{

}

std::vector<packedBufferData> D3DBufferManager::GrabSceneBuffers()
{
	std::vector<packedBufferData> sceneData;
	std::map<Entity*, bufferData>::iterator bdIT;
	for (bdIT = bufferMap.begin() ; bdIT != bufferMap.end() ; ++bdIT)
	{
		packedBufferData newData;
		newData.owningEntity = bdIT->first;
		newData.bufferContents = bdIT->second;
		sceneData.push_back(newData);
	}
	return sceneData;
}