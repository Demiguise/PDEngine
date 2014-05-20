#pragma once
#include "Common.h"
#include "Entity.h"

//Forward Declarations

class Entity;
class RigidBody;
class BoxCollider;
class SphereCollider;

enum ColliderType
{
	Base,
	Box,
	Sphere
};

struct Contact
{
	Contact() {}
	EnVector3 contactPoint;
	EnVector3 contactNormal;
	float penetration;
};

struct CollisionData
{
	CollisionData() {}
	~CollisionData() {
		for (UINT i = 0 ; i < contacts.size() ; ++i)
		{
			if (contacts[i] != 0) { delete contacts [i]; }
		}
	}
	std::vector<Contact*> contacts;
	float restitution;
};

class RigidBody
{
public:
	RigidBody(float initMass, Entity* parentEnt);
	~RigidBody();
	virtual void ReCalculateAABB(BoundingBox& curAABB);

	virtual CollisionData* GenerateContacts(RigidBody* contactingBody);

	Entity* parent;
	EnVector3 centrePoint;
	
	bool collidable;
	bool affectedByGravity;
	float mass;
	UINT typeFlag;
	ModelData rbModel;
};

//All Colliders should inherit from the RigidBody class.

class BoxCollider : public RigidBody
{
public:
	BoxCollider(const ModelData& model, float initMass, Entity* parentEnt);
	~BoxCollider();
	void ReCalculateAABB(BoundingBox& curAABB);
	float extents;
	CollisionData* GenerateContacts(RigidBody* contactingBody);
	EnVector3 halfExtents;
};

class SphereCollider : public RigidBody
{
public:
	SphereCollider(const ModelData& model, float initMass, Entity* parentEnt);
	~SphereCollider();
	void ReCalculateAABB(BoundingBox& curAABB);
	float radius;
	CollisionData* GenerateContacts(RigidBody* contactingBody);
};

namespace CollisionDetectors
{
	bool BoxAndSphere(		const BoxCollider* a,
							const SphereCollider* b,
							std::vector<Contact*>& data);

	bool BoxAndBox(			const BoxCollider* a,
							const BoxCollider* b,
							std::vector<Contact*>& data);

	bool SphereAndSphere(	const SphereCollider* a,
							const SphereCollider* b,
							std::vector<Contact*>& data);

	bool HyperspaceSeperationTest(	const BoxCollider* a,
									const BoxCollider* b,
									const EnVector3& axis);

	float ProjectToAxis(const BoxCollider* a,
						const EnVector3& axis);
}