#include "Colliders.h"



//Base RigidBody
RigidBody::RigidBody(float initMass)
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

CollisionData* RigidBody::GenerateContacts(RigidBody* contactingBody)
{
	return 0;
}


//Box Collider
BoxCollider::BoxCollider(const ModelData& model, float initMass)
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
	centrePoint = curPos;
	EnVector3 curMin = rbModel.vData[0].position + curPos;
	EnVector3 curMax = rbModel.vData[0].position + curPos;
	for (UINT i = 1 ; i < rbModel.vData.size() ; ++i)
	{
		EnVector3 curVertex = rbModel.vData[i].position + curPos;
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
SphereCollider::SphereCollider(const ModelData& model, float initMass)
	: RigidBody(initMass)
{
	typeFlag = ColliderType::Sphere;
	rbModel = model;
}

SphereCollider::~SphereCollider()
{

}

void SphereCollider::ReCalculateAABB(BoundingBox& curAABB, EnVector3 curPos)
{
	centrePoint = curPos;
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
		Contact* newContact = new Contact();
		newContact->contactNormal = EnVector3::Zero();
		newContact->contactPoint = EnVector3::Zero();
		newContact->penetration = 3.145f;
		data.push_back(newContact);
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
		newContact->contactNormal = vectorToTarget.Normalized();
		newContact->contactPoint = a->centrePoint + Util::ScalarProduct3D(vectorToTarget, 0.5f);
		newContact->penetration = (a->radius + b->radius) - penetration;
		data.push_back(newContact);
		return true;
	}

	bool HyperspaceSeperationTest(	const RigidBody* a,
									const RigidBody* b,
									const EnVector3& axis)
	{
		return false;
	}
}