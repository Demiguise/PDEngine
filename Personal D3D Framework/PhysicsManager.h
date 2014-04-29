#pragma once
#include "Entity.h"
#include "Common.h"

class PhysicsManager
{
public:
	PhysicsManager(const std::vector<ModelData>& colliders);
	~PhysicsManager();
	void Update(float dt);
	void CollisionUpdate(float dt);
	bool RayCast(EnVector3 pos, EnVector3 dir, UINT dist = 10);
	void RegisterEntity(Entity* entity, ColliderType rbType, UINT Mass);
	void RemoveEntity(Entity* entity);
	float gravAcceleration;

private:
	std::deque<Entity*> sceneCollideables;
	std::vector<ModelData> colliderModels;
};

