#pragma once
#include "Entity.h"
#include "Common.h"
#include "BufferManager.h"


//This is a abstract base class, I don't want it to ever be called but it provides everything else
//that needs to use a renderer the correct interfaces.

class Renderer
{
public:
	Renderer();
	~Renderer();
	virtual bool Init() = 0;

	virtual void UpdateScene(std::vector<Entity> activeEntities, XMMATRIX* viewMatrix,
							bool refreshBuffers) = 0;
	virtual void DrawScene() = 0;
	virtual void OnResize(UINT newHeight, UINT newWidth) = 0;
	virtual void CreateBuffer(ModelData newModel) = 0;
	virtual void DestroyBuffer() = 0;

	BufferManager* mBufferManager;
	std::string rendererName;
};


#define Pi 3.14f