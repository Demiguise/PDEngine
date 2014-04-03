#pragma once
#include "Entity.h"

class BufferManager
{
public:
	BufferManager(void);
	~BufferManager(void);
	virtual void InitNewBuffer(Entity* newEntity) = 0;
	virtual void DestroyBuffer (Entity* entityToDestroy) = 0;
};

