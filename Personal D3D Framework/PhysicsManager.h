#pragma once
#include "Entity.h"
#include "Common.h"

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	void Update();
	void RegisterEntity(Entity* entity);
	void RemoveEntity(Entity* entity);
	int gravAcceleration;

private:
	std::deque<Entity*> sceneCollideables;
};

