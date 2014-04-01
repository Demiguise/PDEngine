#pragma once
#include "Buffermanager.h"
#include "D3DCommon.h"
#include "Common.h"
#include <D3DX11.h>

class D3DBufferManager :
	public BufferManager
{
public:
	D3DBufferManager(void);
	~D3DBufferManager(void);
	std::map<LPCSTR, bufferData> bufferMap;
	void InitNewBuffer (ID3D11Device* md3dDevice, ModelData newModelData);
};

