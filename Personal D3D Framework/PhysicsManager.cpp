#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(const std::vector<ModelData>& colliders)
{
	gravAcceleration = 9.81f; //ms^-2
	colliderModels = colliders;
	GameLog::Log("[Physics] Initialisation Complete.", DebugLevel::Normal);
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::RegisterEntity(Entity* entity, ColliderType rbType,
									UINT mass)
{
	sceneCollideables.push_back(entity);
	switch (rbType)
	{
	case ColliderType::Box:
		entity->rigidBody = new BoxCollider(colliderModels[0], mass);
		break;
	default:
		entity->rigidBody = new CRigidBody(mass);
		break;
	}
}

void PhysicsManager::RemoveEntity(Entity* entity)
{
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		if (sceneCollideables[i] == entity)
		{
			sceneCollideables.erase(sceneCollideables.begin() + i);
		}
	}
}

void PhysicsManager::Update(float dt)
{
	Entity* curEnt;
	
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		curEnt = sceneCollideables[i];
		curEnt->rigidBody->centrePoint = curEnt->position;
		if (curEnt->rigidBody->affectedByGravity == true)
		{
			curEnt->AddForce(EnVector3(0.0f, -1.0f, 0.0f), (gravAcceleration * curEnt->rigidBody->mass));
		}
		curEnt->position += Util::ScalarProduct3D(curEnt->velocity, dt); //Find the new position from the old velocity.
		EnVector3 accel = Util::ScalarProduct3D(curEnt->forceAccum, (1/curEnt->rigidBody->mass)); // F = MA . Therefor A = F/M
		curEnt->velocity += Util::ScalarProduct3D(accel, dt); //Calculate the new velocity from the acceleration.
		curEnt->forceAccum = EnVector3(0.0f, 0.0f, 0.0f); //Null out the force accumulated in this frame
	}
	CollisionUpdate(dt);
}

void PhysicsManager::CollisionUpdate(float dt)
{
	Entity* curEnt;
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		curEnt = sceneCollideables[i];

	}
}

bool PhysicsManager::RayCast(EnVector3 pos, EnVector3 dir, UINT dist)
{
	//Possibly want to thread this? Unsure as it would make certain objects in other threads be stuck waiting for the results.
	float sampleSize = 0.1f;
	UINT numSamples = dist/sampleSize;
	for (UINT i = 1 ; i <= numSamples ; i++)
	{
		EnVector3 curSamplePos = pos + Util::ScalarProduct3D(dir, (sampleSize * i));
		for (UINT j = 0 ; j < sceneCollideables.size() ; ++j)
		{
				//Do some code.
				//How do I want the information to come back out?
				//Bool for obvious true/false, but what about which object was hit? Unity style 'Out' object?
		}
	}
	return false;
}

