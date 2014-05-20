#pragma once
#include "Common.h"
#include "Colliders.h"
#include "IEventManager.h"
#include "Events.h"
#include "GameLog.h"

class IEventManager;
class RigidBody;


class Entity
{
public:
	Entity(UINT uID, EnVector3 initPos, EnVector3 initRot);
	Entity(UINT uID, EnVector3 initPos);
	Entity(UINT ID);
	~Entity();
	virtual void Update();
	virtual bool OnEvent(IEvent* e);
	void AddForce(EnVector3 direction, float power);
	bool TestAABBIntersection(BoundingBox& incomingAABB);

	std::string name;
	UINT uID;
	EnVector3 position;
	EnVector3 rotation;
	EnMatrix4x4 localToWorld;
	EnVector4 quaternion;
	EnVector3 forceAccum;
	EnVector3 velocity;
	RigidBody* rigidBody;
	BoundingBox AABB;

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
};

