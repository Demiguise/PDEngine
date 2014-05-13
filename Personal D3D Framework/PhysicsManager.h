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

struct CollisionPair
{
	CollisionPair() {}
	CollisionPair(Entity* first, Entity* second)
		: a(first), b(second) {}
	Entity* a;
	Entity* b;
};

class PhysicsManager
{
public:
	PhysicsManager(const std::vector<ModelData>& colliders);
	~PhysicsManager();
	void Update(const float& dt);
	void CollisionUpdate(const float& dt);
	bool CastRay(EnVector3 pos, EnVector3 dir, float dist, RayCastHit& raycastOut);
	void RegisterEntity(Entity* entity, ColliderType rbType, UINT Mass);
	void RemoveEntity(Entity* entity);
	float gravAcceleration;

private:
	void ResolveCollisions(std::vector<CollisionPair>& possibleCollisions,const float& dt);

	std::deque<Entity*> sceneCollideables;
	std::vector<ModelData> colliderModels;
};

