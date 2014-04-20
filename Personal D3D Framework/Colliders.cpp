#include "Colliders.h"

//Base RigidBody

CRigidBody::CRigidBody(UINT initMass)
{
	typeFlag = ColliderType::Base;
	mass = initMass;
	collidable = true;
	affectedByGravity = false;
}

CRigidBody::~CRigidBody()
{

}

bool CRigidBody::CheckForIntersection(EnVector3 pos)
{
	return false;
}

//Box Collider
BoxCollider::BoxCollider(UINT initMass, float scale)
	: CRigidBody(initMass)
{
	typeFlag = ColliderType::Box;
	extents = Util::ScalarProduct3D(EnVector3(1.0f, 1.0f, 1.0f), scale);
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::CheckForIntersection(EnVector3 pos)
{
	EnVector3 minPos = centrePoint - extents;
	EnVector3 maxPos = centrePoint + extents;
	if (pos.x > minPos.x && pos.x < maxPos.x)
	{
		if (pos.y > minPos.y && pos.y < maxPos.y)
		{
			if (pos.z > minPos.z && pos.z < maxPos.z)
			{
				return true;
			}
		}
	}
	return false;
}

