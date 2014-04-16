#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	gravAcceleration = 9.81; //ms^-2
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::RegisterEntity(Entity* entity)
{
	sceneCollideables.push_back(entity);
}

void PhysicsManager::RemoveEntity(Entity* entity)
{
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		if (sceneCollideables[i] == entity)
		{
			sceneCollideables.erase(sceneCollideables.begin() + i);
		}
	}
}

void PhysicsManager::Update()
{
	for (UINT i = 0 ; sceneCollideables.size() ; ++i)
	{

	}
}

