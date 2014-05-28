#include "Entity.h"

Entity::Entity(UINT ID, EnVector3 initPos, EnVector3 initRot)
{
	uID = ID;
	position = initPos;
	rotation = initRot;
	Init();
}

Entity::Entity(UINT ID, EnVector3 initPos)
{
	uID = ID;
	position = initPos;
	rotation = EnVector3::Zero();
	Init();
}

Entity::Entity(UINT ID)
{
	uID = ID;
	position = EnVector3::Zero();
	rotation = EnVector3::Zero();
	Init();
}

Entity::~Entity()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
	delete rigidBody;
}

void Entity::Init()
{
	forceAccum = EnVector3::Zero();
	velocity = EnVector3::Zero();
	AABB = BoundingBox();
	rigidBody = 0;
}

//Runtime

//Performs basic update operations all Entities will require
void Entity::Update()
{
	UpdateQuaternion();
	UpdateLocalToWorldMatrix();
	//Update our AABB to our current co-ordinates.
	if (rigidBody != 0)
	{
		rigidBody->ReCalculateAABB(AABB);
	}
}

void Entity::UpdateQuaternion()
{
	Quaternion xRotation(rotation.x, EnVector3(1.0f,0.0f,0.0f));
	Quaternion yRotation(rotation.y, EnVector3(0.0f,1.0f,0.0f));
	Quaternion zRotation(rotation.z, EnVector3(0.0f,0.0f,1.0f));
	quaternion = (xRotation*yRotation*zRotation).Normalized();
}

void Entity::UpdateLocalToWorldMatrix()
{
	EnMatrix3x3 rotationMatrix = quaternion.To3x3Matrix();
	for (UINT i = 0 ; i < 3 ; ++i)
	{
		for (UINT j = 0 ; j < 3 ; ++j)
		{
			localToWorld.c[i][j] = rotationMatrix.c[i][j];
		}
	}
	localToWorld.c[3] = EnVector4(position.x, position.y, position.z, 1.0f);
}

EnVector3 Entity::GetLocalAxis(const int& index)
{
	assert ((index <= 3) && (index >= 0));
	switch (index)
	{
	case 0:
		return EnVector3(localToWorld.c[0].x, localToWorld.c[0].y, localToWorld.c[0].z);
	case 1:
		return EnVector3(localToWorld.c[1].x, localToWorld.c[1].y, localToWorld.c[1].z);
	case 2:
		return EnVector3(localToWorld.c[2].x, localToWorld.c[2].y, localToWorld.c[2].z);
	case 3:
		return EnVector3(localToWorld.c[3].x, localToWorld.c[3].y, localToWorld.c[3].z);
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