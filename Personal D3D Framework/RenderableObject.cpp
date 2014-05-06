#include "RenderableObject.h"


CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh, EnVector3 initPos, EnVector3 initRot) 
	: Entity (uID, initPos, initRot)
{
	mesh = newMesh;
}

CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh, EnVector3 initPos) 
	: Entity (uID, initPos)
{
	mesh = newMesh;
}

CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh) 
	: Entity (uID)
{
	mesh = newMesh;
}

CRenderableObject::~CRenderableObject()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}

void CRenderableObject::Update()
{
	if (rigidBody != 0)
	{
		rigidBody->ReCalculateAABB(AABB, position);
	}
}