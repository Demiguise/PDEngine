#include "RenderableObject.h"


CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh, EnVector3 initPos, EnVector3 initRot) 
	: Entity (uID, initPos, initRot)
{
	mesh = newMesh;
	position = initPos;
	rotation = initRot;
}

CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh, EnVector3 initPos) 
	: Entity (uID, initPos)
{
	mesh = newMesh;
	position = initPos;
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
}

CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh) 
	: Entity (uID)
{
	mesh = newMesh;
	position = EnVector3(0.0f, 0.0f, 0.0f);
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
}

CRenderableObject::~CRenderableObject()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}
