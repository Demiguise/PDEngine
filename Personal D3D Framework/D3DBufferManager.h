#pragma once
#include "Buffermanager.h"
#include "D3DCommon.h"
#include "Common.h"
#include <D3DX11.h>

class D3DBufferManager :
	public BufferManager
{
public:
	D3DBufferManager(ID3D11Device* mDevice);
	~D3DBufferManager();
	void InitNewBuffer (Entity* newEntity);
	void DestroyBuffer (Entity* entityToDestroy);
	std::vector<packedBufferData> GrabSceneBuffers ();
	std::map<Entity*, bufferData> bufferMap;

private:
	ID3D11Device* md3dDevice;
};

