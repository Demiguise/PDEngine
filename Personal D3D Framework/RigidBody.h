#pragma once
#include "Common.h"

class CRigidBody
{
public:
	CRigidBody();
	~CRigidBody();

	bool collidable;
	bool affectedByGravity;
	UINT mass; //We wouldn't ever want negative mass, I shouldn't think.
};

