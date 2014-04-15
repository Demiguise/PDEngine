#pragma once
#include "RenderableObject.h"

class BufferManager
{
public:
	BufferManager(void);
	~BufferManager(void);
	virtual void InitNewBuffer(CRenderableObject* newEntity) = 0;
	virtual void DestroyBuffer (CRenderableObject* entityToDestroy) = 0;
};

