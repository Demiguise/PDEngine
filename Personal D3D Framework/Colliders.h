#pragma once
#include "Common.h"

enum ColliderType
{
	Base,
	Box
};

class CRigidBody
{
public:
	CRigidBody(UINT initMass);
	~CRigidBody();
	EnVector3 centrePoint;
	bool collidable;
	bool affectedByGravity;
	UINT mass; //We wouldn't ever want negative mass, I shouldn't think.
	UINT typeFlag;
};

//All Colliders should inherit from the CRigidBody class.
//I'm super bad at keeping naming conventions.
class BoxCollider : public CRigidBody
{
public:
	BoxCollider(const ModelData& model, UINT initMass);
	~BoxCollider();
	bool CheckForIntersection(EnVector3 pos, EnVector3 dir, EnVector3& intersectPoint);
	
	ModelData rbModel;
};

