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
		entity->rigidBody = new BoxCollider(colliderModels[0], mass);
		break;
	default:
		entity->rigidBody = new RigidBody(mass);
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
	std::vector<Entity*> possibleCollisions;
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		curEnt = sceneCollideables[i];
		possibleCollisions.clear();
		//The 'Velocity AABB' is a bounding box that has been scaled up proportionally with the objects velocity.
		//It's an incredibly coarse method and I would prefer it to be something else, but this will work for now.
		BoundingBox velocityAABB;
		velocityAABB.minPoint = curEnt->AABB.minPoint + Util::ScalarProduct3D((curEnt->AABB.minPoint - curEnt->AABB.maxPoint), curEnt->velocity.GetMagnitude() * dt);
		velocityAABB.maxPoint = curEnt->AABB.maxPoint + Util::ScalarProduct3D((curEnt->AABB.maxPoint - curEnt->AABB.minPoint), curEnt->velocity.GetMagnitude() * dt);
		for (UINT j = 0 ; j < sceneCollideables.size() ; ++j)
		{
			if ((i != j) && (sceneCollideables[j]->TestAABBIntersection(velocityAABB)))
			{
				possibleCollisions.push_back(sceneCollideables[j]);
			}
		}
		if (!possibleCollisions.empty())
		{
			EnMatrix3x3 asd;
			RayCastHit CollisionInfo;
			if (RayCastToEntities(	curEnt->position, 
									curEnt->velocity.GetMagnitude(), 
									possibleCollisions, CollisionInfo))
			{
				curEnt->AddForce(CollisionInfo.normal.Normalized(), 250);
				CollisionInfo.entityHit->AddForce(Util::ScalarProduct3D(CollisionInfo.normal.Normalized(), -1), 250);
				GameLog::GetInstance()->Log(DebugChannel::Physics, DebugLevel::Normal, "[Physics] %s will collide with %s next frame.", curEnt->name.c_str(), CollisionInfo.entityHit->name.c_str());
			}
		}
	}
}


bool PhysicsManager::RayCastToEntities(EnVector3 pos, float dist, std::vector<Entity*>& testableEntities, RayCastHit& raycastOut)
{
	std::vector<RayCastHit> contactInfos;
	ModelData curRigidBodyMdl;
	EnMatrix3x3 linePlanematrix;
	EnVector3 linePlaneVector;
	EnVector3 linePlaneCoefficients;
	for (UINT i = 0 ; i < testableEntities.size() ; ++i)
	{
		curRigidBodyMdl = testableEntities[i]->rigidBody->rbModel;
		//Jep, testing every single triangle! Fun times!
		//Testing every other triangle would work in this instance, unsure how that might apply. Changing to J+6 should do the trick.
		//I feel bad about just assuming the model data would work like this.
		for (UINT j = 0 ; j < curRigidBodyMdl.iData.size() ; j += 3) 
		{
			//==============
			//TODO:
			//Need to take into account the rotation of the object as well
			UINT v1 = curRigidBodyMdl.iData[j];
			UINT v2 = curRigidBodyMdl.iData[j+1];
			UINT v3 = curRigidBodyMdl.iData[j+2];
			linePlaneVector = pos - curRigidBodyMdl.vData[v1].position;
			linePlanematrix.r[0] = pos - Util::ScalarProduct3D(testableEntities[i]->position.Normalized(), dist);
			linePlanematrix.r[1] = curRigidBodyMdl.vData[v2].position - curRigidBodyMdl.vData[v1].position;
			linePlanematrix.r[2] = curRigidBodyMdl.vData[v3].position - curRigidBodyMdl.vData[v1].position;
			if (linePlanematrix.Invert()) //Line is intersecting the plane.
			{
				linePlaneCoefficients = linePlaneVector.MatrixMult3x3(linePlanematrix);
				if((linePlaneCoefficients.x > 0) && (linePlaneCoefficients.x < 1)) //Lies within our lines maximum distance
				{
					contactInfos.push_back(RayCastHit(testableEntities[i],
											(pos + Util::ScalarProduct3D(testableEntities[i]->position.Normalized(), linePlaneCoefficients.x)),
											linePlanematrix.r[1].Cross(linePlanematrix.r[2])));
				}

			}
			else //Line is parallel to the plane.
			{
				//==============
				//TODO:
				//Some extra checking to ensure it's not inside the actual plane.
			}
		}
	}
	if (!contactInfos.empty())
	{
		if (contactInfos.size() == 1) //Easy out
		{ 
			raycastOut = contactInfos[0];
			return true; 
		} 
		float minDist = dist;
		int posIndex = 0;
		for (UINT i = 0 ; i < contactInfos.size() ; ++i)
		{
			float distToContact = (pos - contactInfos[i].collisionPos).GetMagnitude();
			if ( distToContact < minDist) 
			{ 
				minDist = distToContact;
				posIndex = i; 
			}
		}
		raycastOut = contactInfos[posIndex];
		return true;
	}
	return false;
}

