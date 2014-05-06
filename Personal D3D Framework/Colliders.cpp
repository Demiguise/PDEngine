#include "Colliders.h"



//Base RigidBody
RigidBody::RigidBody(UINT initMass)
{
	typeFlag = ColliderType::Base;
	mass = initMass;
	collidable = true;
	affectedByGravity = false;
}

RigidBody::~RigidBody()
{

}

void RigidBody::ReCalculateAABB(BoundingBox& curAABB, EnVector3 curPos)
{

}

//Box Collider
BoxCollider::BoxCollider(const ModelData& model, UINT initMass)
	: RigidBody(initMass)
{
	typeFlag = ColliderType::Box;
	rbModel = model;
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::ReCalculateAABB(BoundingBox& curAABB, EnVector3 curPos)
{
	EnVector3 curMin = EnVector3();
	EnVector3 curMax = EnVector3();
	for (UINT i = 0 ; i < rbModel.vData.size() ; ++i)
	{
		EnVector3 curVertex = rbModel.vData[i].position + curPos;
		if (curVertex.x > curMax.x) { curMax.x = curVertex.x; }
		if (curVertex.y > curMax.y) { curMax.y = curVertex.y; }
		if (curVertex.z > curMax.z) { curMax.z = curVertex.z; }
		if (curVertex.x < curMin.x) { curMin.x = curVertex.x; }
		if (curVertex.y < curMin.y) { curMin.y = curVertex.y; }
		if (curVertex.z < curMax.z) { curMin.z = curVertex.z; }
	}
	curAABB.minPoint = curMin;
	curAABB.maxPoint = curMax;
}