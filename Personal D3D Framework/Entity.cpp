#include "Entity.h"

Entity::Entity(UINT ID, EnVector3 initPos, EnVector3 initRot)
{
	uID = ID;
	position = initPos;
	rotation = initRot;
	forceAccum = EnVector3::Zero();
	velocity = EnVector3::Zero();
	AABB = BoundingBox();
	rigidBody = 0;
}

Entity::Entity(UINT ID, EnVector3 initPos)
{
	uID = ID;
	position = initPos;
	rotation = EnVector3::Zero();
	forceAccum = EnVector3::Zero();
	velocity = EnVector3::Zero();
	AABB = BoundingBox();
	rigidBody = 0;
}

Entity::Entity(UINT ID)
{
	uID = ID;
	position = EnVector3::Zero();
	rotation = EnVector3::Zero();
	forceAccum = EnVector3::Zero();
	velocity = EnVector3::Zero();
	AABB = BoundingBox();
	rigidBody = 0;
}

Entity::~Entity()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
	delete rigidBody;
}

//Runtime

void Entity::Update()
{
	//Update our AABB to our current co-ordinates.
	if (rigidBody != 0)
	{
		rigidBody->ReCalculateAABB(AABB);
	}
}

bool Entity::OnEvent(IEvent* e)
{
	return false;
}

void Entity::AddForce(EnVector3 direction, float power)
{
	forceAccum += Util::ScalarProduct3D(direction, power);
}

bool Entity::TestAABBIntersection(BoundingBox& incomingAABB)
{
	//Debug Intersection code
	//bool A =	AABB.maxPoint.x > incomingAABB.minPoint.x;
	//bool B =	AABB.minPoint.x < incomingAABB.maxPoint.x;
	//bool C =	AABB.maxPoint.y > incomingAABB.minPoint.y;
	//bool D =	AABB.minPoint.y < incomingAABB.maxPoint.y;
	//bool E =	AABB.maxPoint.z > incomingAABB.minPoint.z;
	//bool F =	AABB.minPoint.z < incomingAABB.maxPoint.z;
	//return false;
	return (AABB.maxPoint.x > incomingAABB.minPoint.x &&
			AABB.minPoint.x < incomingAABB.maxPoint.x &&
			AABB.maxPoint.y > incomingAABB.minPoint.y &&
			AABB.minPoint.y < incomingAABB.maxPoint.y &&
			AABB.maxPoint.z > incomingAABB.minPoint.z &&
			AABB.minPoint.z < incomingAABB.maxPoint.z);
}