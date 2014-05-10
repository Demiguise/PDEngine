#pragma once
#include "Common.h"

enum ColliderType
{
	Base,
	Box
};

class RigidBody
{
public:
	RigidBody(UINT initMass);
	~RigidBody();
	virtual void ReCalculateAABB(BoundingBox& curAABB, EnVector3 curPos);

	EnVector3 centrePoint;
	bool collidable;
	bool affectedByGravity;
	UINT mass; //We wouldn't ever want negative mass, I shouldn't think.
	UINT typeFlag;
	ModelData rbModel;
};

//All Colliders should inherit from the RigidBody class.
class BoxCollider : public RigidBody
{
public:
	BoxCollider(const ModelData& model, UINT initMass);
	~BoxCollider();
	void ReCalculateAABB(BoundingBox& curAABB, EnVector3 curPos);
};