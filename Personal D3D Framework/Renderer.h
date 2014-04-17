#pragma once
#include "RenderableObject.h"
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

	virtual void UpdateScene(const EnVector3 &camPos) = 0;
	virtual void DrawScene() = 0;
	virtual void OnResize(UINT newHeight, UINT newWidth) = 0;
	virtual void CreateBuffer(CRenderableObject* newEnt) = 0;
	virtual void DestroyBuffer(CRenderableObject* entity) = 0;
	std::string rendererName;
};