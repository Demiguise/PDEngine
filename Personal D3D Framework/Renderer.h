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

	virtual void UpdateScene(XMMATRIX* viewMatrix) = 0;
	virtual void DrawScene() = 0;
	virtual void OnResize(UINT newHeight, UINT newWidth) = 0;
	virtual void CreateBuffer(Entity* newEnt) = 0;
	virtual void DestroyBuffer(Entity* entity) = 0;
	std::string rendererName;
};


#define Pi 3.14f