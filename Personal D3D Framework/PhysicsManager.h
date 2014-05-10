#pragma once
#include "Entity.h"
#include "Common.h"

struct RayCastHit
{
	RayCastHit()
		: entityHit(0), collisionPos(EnVector3::Zero()), normal(EnVector3::Zero()) {}
	RayCastHit(Entity* e, EnVector3 p, EnVector3 n)
		: entityHit(e), collisionPos(p), normal(n) {}
	Entity* entityHit;
	EnVector3 collisionPos;
	EnVector3 normal;
};

class PhysicsManager
{
public:
	PhysicsManager(const std::vector<ModelData>& colliders);
	~PhysicsManager();
	void Update(float dt);
	void CollisionUpdate(float dt);
	bool RayCastToEntities(EnVector3 pos, float dist, std::vector<Entity*>& testableEntities, RayCastHit& raycastOut);
	void RegisterEntity(Entity* entity, ColliderType rbType, UINT Mass);
	void RemoveEntity(Entity* entity);
	float gravAcceleration;

private:
	std::deque<Entity*> sceneCollideables;
	std::vector<ModelData> colliderModels;
};

