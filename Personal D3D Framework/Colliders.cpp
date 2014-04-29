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


//Box Collider
BoxCollider::BoxCollider(const ModelData& model, UINT initMass)
	: CRigidBody(initMass)
{
	typeFlag = ColliderType::Box;
	rbModel = model;
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::CheckForIntersection(EnVector3 pos, EnVector3 dir, EnVector3& intersectPoint)
{
	return false;
}
