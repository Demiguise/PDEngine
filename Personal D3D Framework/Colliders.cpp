#include "Colliders.h"



//Base RigidBody
RigidBody::RigidBody(float initMass, Entity* parentEnt)
{
	typeFlag = ColliderType::Base;
	mass = initMass;
	collidable = true;
	affectedByGravity = false;
	parent = parentEnt;
	isAwake = true;
}

RigidBody::~RigidBody()
{

}

void RigidBody::ReCalculateAABB(BoundingBox& curAABB)
{

}

CollisionData* RigidBody::GenerateContacts(RigidBody* contactingBody)
{
	return 0;
}


//Box Collider
BoxCollider::BoxCollider(const ModelData& model, float initMass, Entity* parentEnt)
	: RigidBody(initMass, parentEnt)
{
	typeFlag = ColliderType::Box;
	rbModel = model;
	//HACK : I need to be able to calculate this rather than set it. In the case of scaling and such.
	halfExtents = EnVector3(1.0f, 1.0f, 1.0f);
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::ReCalculateAABB(BoundingBox& curAABB)
{
	EnVector3 curMin = rbModel.vData[0].position.MatrixMult4x4(parent->localToWorld);
	EnVector3 curMax = rbModel.vData[0].position.MatrixMult4x4(parent->localToWorld);
	for (UINT i = 1 ; i < rbModel.vData.size() ; ++i)
	{
		EnVector3 curVertex = rbModel.vData[i].position.MatrixMult4x4(parent->localToWorld);
		if (curVertex.x > curMax.x) { curMax.x = curVertex.x; }
		if (curVertex.y > curMax.y) { curMax.y = curVertex.y; }
		if (curVertex.z > curMax.z) { curMax.z = curVertex.z; }
		if (curVertex.x < curMin.x) { curMin.x = curVertex.x; }
		if (curVertex.y < curMin.y) { curMin.y = curVertex.y; }
		if (curVertex.z < curMin.z) { curMin.z = curVertex.z; }
	}
	curAABB.minPoint = curMin;
	curAABB.maxPoint = curMax;
}

//Contact Generation
CollisionData* BoxCollider::GenerateContacts(RigidBody* contactingBody)
{
	CollisionData* data = new CollisionData();
	std::vector<Contact*> contacts;
	switch (contactingBody->typeFlag)
	{
	case ColliderType::Base:
		break;
	case ColliderType::Box:
		if (CollisionDetectors::BoxAndBox(this, static_cast<BoxCollider*>(contactingBody), contacts))
		{
			data->contacts = contacts;
			return data;
		}
		break;
	case ColliderType::Sphere:
		if (CollisionDetectors::BoxAndSphere(this, static_cast<SphereCollider*>(contactingBody), contacts))
		{
			data->contacts = contacts;
			return data;
		}
		break;
	}
	return 0;
}

//Sphere Collider
SphereCollider::SphereCollider(const ModelData& model, float initMass, Entity* parentEnt)
	: RigidBody(initMass, parentEnt)
{
	typeFlag = ColliderType::Sphere;
	rbModel = model;
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::ReCalculateAABB(BoundingBox& curAABB)
{
	
}

//Contact Generation

CollisionData* SphereCollider::GenerateContacts(RigidBody* contactingBody)
{
	CollisionData* data = new CollisionData();
	std::vector<Contact*> contacts;
	switch (contactingBody->typeFlag)
	{
	case ColliderType::Base:
		break;
	case ColliderType::Box:
		if (CollisionDetectors::BoxAndSphere(static_cast<BoxCollider*>(contactingBody), this, contacts))
		{

			data->contacts = contacts;
			return data;
		}
		break;
	case ColliderType::Sphere:
		if (CollisionDetectors::SphereAndSphere(this, static_cast<SphereCollider*>(contactingBody), contacts))
		{
			data->contacts = contacts;
			return data;
		}
		break;
	}
	return 0;
}

namespace CollisionDetectors
{
	bool BoxAndSphere(	const BoxCollider* a,
						const SphereCollider* b,
						std::vector<Contact*>& data)
	{
		return false;
	}

	bool BoxAndBox(	const BoxCollider* a,
					const BoxCollider* b,
					std::vector<Contact*>& data)
	{
		if (HyperplaneSeperationTest(a,b))
		{
			Contact* newContact = new Contact();
			Contact* tempContact = new Contact();
			//We'll iterate through all the points in the box A and find the point of deepest interpenetration
			for (int i = 0 ; i < a->rbModel.vData.size() ; ++i)
			{
				if ((BoxAndPoint(a->rbModel.vData[i].position.MatrixMult4x4(a->parent->localToWorld), b, *tempContact)) && (tempContact->penetration > newContact->penetration))
				{
					*newContact = *tempContact;
				}
			}
			data.push_back(newContact);
			delete tempContact;
			return true;
		}
		return false;
	}

	bool BoxAndPoint(	const EnVector3& p,
						const BoxCollider* b,
						Contact& data)
	{
		EnVector3 relativePoint = b->parent->localToWorld.RotationalInverse(p);
		EnVector3 normal;
		float minimumDepth = b->halfExtents.x - abs(relativePoint.x);
		float depth;

		if (minimumDepth < 0) { return false; }
		normal = Util::ScalarProduct3D(b->parent->GetLocalAxis(0), ((relativePoint.x < 0)?-1:1));

		depth = b->halfExtents.y - abs(relativePoint.y);
		if (depth < 0) { return false; }
		else if (depth < minimumDepth)
		{
			minimumDepth = depth;
			normal = Util::ScalarProduct3D(b->parent->GetLocalAxis(1), ((relativePoint.y < 0)?-1:1));
		}

		depth = b->halfExtents.z - abs(relativePoint.z);
		if (depth < 0) { return false; }
		else if (depth < minimumDepth)
		{
			minimumDepth = depth;
			normal = Util::ScalarProduct3D(b->parent->GetLocalAxis(2), ((relativePoint.z < 0)?-1:1));
		}

		data.contactNormal = normal;
		data.contactPoint = p;
		data.penetration = minimumDepth;

		return true;
	}


	bool SphereAndSphere(	const SphereCollider* a,
							const SphereCollider* b,
							std::vector<Contact*>& data)
	{
		EnVector3 vectorToTarget = b->centrePoint - a->centrePoint;
		float penetration = vectorToTarget.GetMagnitude();
		if (penetration <= 0.0f || penetration >= a->radius + b->radius) { return false; }

		Contact* newContact = new Contact();
		newContact->contactNormal = Util::ScalarProduct3D(vectorToTarget.Normalized(), -1.0f);
		newContact->contactPoint = a->centrePoint + Util::ScalarProduct3D(vectorToTarget, 0.5f);
		newContact->penetration = (a->radius + b->radius) - penetration;
		data.push_back(newContact);
		return true;
	}

	bool HyperplaneSeperationTest(	const BoxCollider* a,
									const BoxCollider* b)
	{
		//Now for the actual testing.
				//Box A's Axes
		return (TestForOverlap(a,b, a->parent->GetLocalAxis(0)) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(1)) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(2)) &&
				//Box b's axes
				TestForOverlap(a,b, b->parent->GetLocalAxis(0)) &&
				TestForOverlap(a,b, b->parent->GetLocalAxis(1)) &&
				TestForOverlap(a,b, b->parent->GetLocalAxis(2)) &&
				//Cross products of each axes
				TestForOverlap(a,b, a->parent->GetLocalAxis(0).Cross(b->parent->GetLocalAxis(0))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(0).Cross(b->parent->GetLocalAxis(1))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(0).Cross(b->parent->GetLocalAxis(2))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(1).Cross(b->parent->GetLocalAxis(0))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(1).Cross(b->parent->GetLocalAxis(1))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(1).Cross(b->parent->GetLocalAxis(2))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(2).Cross(b->parent->GetLocalAxis(0))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(2).Cross(b->parent->GetLocalAxis(1))) &&
				TestForOverlap(a,b, a->parent->GetLocalAxis(2).Cross(b->parent->GetLocalAxis(2))));

		////Debug return code. For when I need to test things...
		////Box A's Axes
		//bool aX1 = TestForOverlap(a,b, a->parent->GetLocalAxis(0));
		//bool aX2 = TestForOverlap(a,b, a->parent->GetLocalAxis(1));
		//bool aX3 = TestForOverlap(a,b, a->parent->GetLocalAxis(2));
		////Box b's axes
		//bool bX1 = TestForOverlap(a,b, b->parent->GetLocalAxis(0));
		//bool bX2 = TestForOverlap(a,b, b->parent->GetLocalAxis(1));
		//bool bX3 = TestForOverlap(a,b, b->parent->GetLocalAxis(2));
		////Cross products of each axes
		//bool abX11 = TestForOverlap(a,b, a->parent->GetLocalAxis(0).Cross(b->parent->GetLocalAxis(0)));
		//bool abX12 = TestForOverlap(a,b, a->parent->GetLocalAxis(0).Cross(b->parent->GetLocalAxis(1))); 
		//bool abX13 = TestForOverlap(a,b, a->parent->GetLocalAxis(0).Cross(b->parent->GetLocalAxis(2))); 
		//bool abX21 = TestForOverlap(a,b, a->parent->GetLocalAxis(1).Cross(b->parent->GetLocalAxis(0))); 
		//bool abX22 = TestForOverlap(a,b, a->parent->GetLocalAxis(1).Cross(b->parent->GetLocalAxis(1))); 
		//bool abX23 = TestForOverlap(a,b, a->parent->GetLocalAxis(1).Cross(b->parent->GetLocalAxis(2))); 
		//bool abX31 = TestForOverlap(a,b, a->parent->GetLocalAxis(2).Cross(b->parent->GetLocalAxis(0))); 
		//bool abX32 = TestForOverlap(a,b, a->parent->GetLocalAxis(2).Cross(b->parent->GetLocalAxis(1))); 
		//bool abX33 = TestForOverlap(a,b, a->parent->GetLocalAxis(2).Cross(b->parent->GetLocalAxis(2)));
		//return true;
	}

	bool TestForOverlap(	const BoxCollider* a,
							const BoxCollider* b,
							const EnVector3& axis)
	{
		float aProjection = ProjectToAxis(a, axis);
		float bProjection = ProjectToAxis(b, axis);
		EnVector3 abVector = b->parent->GetLocalAxis(3) - a->parent->GetLocalAxis(3);
		float abVectorProjection = abs(abVector.ADot(axis));
		return abVectorProjection <= (aProjection + bProjection);
	}


	float ProjectToAxis(const BoxCollider* a,
						const EnVector3& axis)
	{
		return (	abs(a->halfExtents.x * axis.ADot(a->parent->GetLocalAxis(0))) +
					abs(a->halfExtents.y * axis.ADot(a->parent->GetLocalAxis(1))) +
					abs(a->halfExtents.z * axis.ADot(a->parent->GetLocalAxis(2))));
	}
}