#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(const std::vector<ModelData>& colliders)
{
	gravAcceleration = 9.81f; //ms^-2
	colliderModels = colliders;
	GameLog::GetInstance()->Log(DebugChannel::Physics, DebugLevel::Normal, "[Physics] Initialisation Complete.");
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
		entity->rigidBody = new BoxCollider(colliderModels[0], mass, entity);
		break;
	default:
		entity->rigidBody = new RigidBody(mass, entity);
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

void PhysicsManager::Update(const float& dt)
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
		//GameLog::GetInstance()->Log(DebugChannel::Physics, DebugLevel::Normal, "[Physics] %s | Velocity is (%f, %f, %f).", curEnt->name.c_str(), curEnt->velocity.x, curEnt->velocity.y, curEnt->velocity.z);
	}
	CollisionUpdate(dt);
}

void PhysicsManager::CollisionUpdate(const float& dt)
{
	//First off, let's begin our coarse collision detection. This tests for only intersections in each frame, no fancy stuff.
	std::vector<CollisionPair> sceneCollisions = CoarseCollisionDetection(sceneCollideables);

	if (!sceneCollisions.empty())
	{
		GenerateContacts(sceneCollisions);
		ResolveCollisions(sceneCollisions, dt);
	}
}

std::vector<CollisionPair> PhysicsManager::CoarseCollisionDetection(const std::deque<Entity*>& availableCollideables)
{
	Entity* curEnt;
	std::vector<CollisionPair> possibleCollisions;
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		curEnt = sceneCollideables[i];
		possibleCollisions.clear();
		for (UINT j = 0 ; j < sceneCollideables.size() ; ++j)
		{
			if ((i != j) && (sceneCollideables[j]->TestAABBIntersection(curEnt->AABB))) 
			{ //Collision between two entities has occured.
				bool collisionIsUnique = true;
				for (UINT k = 0 ; k < possibleCollisions.size() ; ++k)
				{
					if((possibleCollisions[k].a == sceneCollideables[j]) && (possibleCollisions[k].b == curEnt))
					{ //Already a collision between these two entities in the system, ignore it.
						collisionIsUnique = false;
					}
				}
				if (collisionIsUnique) { possibleCollisions.push_back(CollisionPair(curEnt, sceneCollideables[j])); }
			}
		}
	}
	return possibleCollisions;
}

void PhysicsManager::GenerateContacts(std::vector<CollisionPair>& coarseCollisions)
{
	for (UINT i = 0 ; i < coarseCollisions.size() ; ++i)
	{
		CollisionData* contacts = coarseCollisions[i].a->rigidBody->GenerateContacts(coarseCollisions[i].b->rigidBody);
		coarseCollisions[i].data = contacts;
	}
}

void PhysicsManager::ResolveCollisions(std::vector<CollisionPair>& possibleCollisions, const float& dt)
{
	//for ( UINT i = 0 ; i < possibleCollisions.size() ; ++i)
	//{
	//	if (possibleCollisions[i].data != 0) { continue; }
	//	RayCastHit CollisionInfo;
	//	Entity* entA = possibleCollisions[i].a;
	//	Entity* entB = possibleCollisions[i].b;
	//	EnVector3 directionVector = entB->position - entA->position;
	//	if (CastRay(entA->position, directionVector,
	//				Util::ScalarProduct3D(entA->velocity, dt).GetMagnitude(), 
	//				CollisionInfo))
	//	{	//If the ray hit an object within the distance given then an actual collision has occured, resolve it.
	//		//Remember that entB might not actually be the Entity hit.

	//		float restitution = 0.0f; //Controls the elasticity of the collisions, 0 = inelastic & 1 = elastic.
	//		float seperatingVelocity = (entA->velocity - CollisionInfo.entityHit->velocity).ADot(CollisionInfo.normal);
	//		float deltaVelocity = (-seperatingVelocity * restitution) - seperatingVelocity;
	//		float totalInverseMass = 1/(entA->rigidBody->mass + CollisionInfo.entityHit->rigidBody->mass);
	//		float impulse = deltaVelocity / totalInverseMass;
	//		EnVector3 impulsePerIMass = Util::ScalarProduct3D(CollisionInfo.normal, impulse);

	//		entA->velocity += Util::ScalarProduct3D(impulsePerIMass, 1/entA->rigidBody->mass);
	//		CollisionInfo.entityHit->velocity += Util::ScalarProduct3D(impulsePerIMass, 1/-CollisionInfo.entityHit->rigidBody->mass);
	//		GameLog::GetInstance()->Log(DebugChannel::Physics, DebugLevel::Normal, "[Physics] %s will collide with %s next frame.", entA->name.c_str(), CollisionInfo.entityHit->name.c_str());
	//	}
	//}
}

bool PhysicsManager::CastRay(EnVector3 pos, EnVector3 dir,  float dist, RayCastHit& raycastOut)
{
	//std::vector<RayCastHit> contactInfos;
	//ModelData curRigidBodyMdl;
	//EnMatrix3x3 linePlaneMatrix;
	//EnVector3 linePlaneVector;
	//EnVector3 linePlaneCoefficients;
	//curRigidBodyMdl = entityrigidBody->rbModel;
	////Jep, testing every single triangle! Fun times!
	////Testing every other triangle would work in this instance, unsure how that might apply. Changing to J+6 should do the trick.
	////I feel bad about just assuming the model data would work like this.
	//for (UINT j = 0 ; j < curRigidBodyMdl.iData.size() ; j += 3) 
	//{
	//	//==============
	//	//TODO:
	//	//Need to take into account the rotation of the object as well
	//	UINT v1 = curRigidBodyMdl.iData[j];
	//	UINT v2 = curRigidBodyMdl.iData[j+1];
	//	UINT v3 = curRigidBodyMdl.iData[j+2];
	//	linePlaneVector = pos - curRigidBodyMdl.vData[v1].position;
	//	linePlaneMatrix.r[0] = Util::ScalarProduct3D((testableEntities[i]->position - pos), dist);
	//	linePlaneMatrix.r[1] = (testableEntities[i]->position + curRigidBodyMdl.vData[v2].position) - (testableEntities[i]->position + curRigidBodyMdl.vData[v1].position);
	//	linePlaneMatrix.r[2] = (testableEntities[i]->position + curRigidBodyMdl.vData[v3].position) - (testableEntities[i]->position + curRigidBodyMdl.vData[v1].position);
	//	if (linePlaneMatrix.Invert()) //Line is intersecting the plane.
	//	{
	//		linePlaneCoefficients = linePlaneVector.MatrixMult3x3(linePlaneMatrix);
	//		if((linePlaneCoefficients.x > 0) && (linePlaneCoefficients.x < 1)) //Lies within our lines maximum distance
	//		{
	//			EnVector3 collisionNormal = (curRigidBodyMdl.vData[v2].position - curRigidBodyMdl.vData[v1].position).Cross(curRigidBodyMdl.vData[v3].position - curRigidBodyMdl.vData[v1].position).Normalized();
	//			contactInfos.push_back(RayCastHit(testableEntities[i],
	//									(pos + Util::ScalarProduct3D((testableEntities[i]->position - pos).Normalized(), linePlaneCoefficients.x)),
	//									collisionNormal));
	//		}

	//	}
	//	else //Line is parallel to the plane.
	//	{
	//		//==============
	//		//TODO:
	//		//Some extra checking to ensure it's not inside the actual plane.
	//	}
	//}
	//if (!contactInfos.empty())
	//{
	//	if (contactInfos.size() == 1) //Easy out
	//	{ 
	//		raycastOut = contactInfos[0];
	//		return true; 
	//	} 
	//	float minDist = dist;
	//	int posIndex = 0;
	//	for (UINT i = 0 ; i < contactInfos.size() ; ++i)
	//	{
	//		float distToContact = (pos - contactInfos[i].collisionPos).GetMagnitude();
	//		if ( distToContact < minDist) 
	//		{ 
	//			minDist = distToContact;
	//			posIndex = i; 
	//		}
	//	}
	//	raycastOut = contactInfos[posIndex];
	//	return true;
	//}
	return false;
}

