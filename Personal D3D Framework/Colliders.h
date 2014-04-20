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
	virtual bool CheckForIntersection(EnVector3 pos);

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
	BoxCollider(UINT initMass, float scale);
	~BoxCollider();
	bool CheckForIntersection(EnVector3 pos);
	
	EnVector3 extents;
};

