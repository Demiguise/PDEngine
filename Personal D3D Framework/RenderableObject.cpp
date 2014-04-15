#include "RenderableObject.h"


CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh, XMFLOAT3 initPos, XMFLOAT3 initRot) 
	: Entity (uID, initPos, initRot)
{
	mesh = newMesh;
	position = initPos;
	rotation = initRot;
}

CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh, XMFLOAT3 initPos) 
	: Entity (uID, initPos)
{
	mesh = newMesh;
	position = initPos;
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CRenderableObject::CRenderableObject(UINT uID, ModelData newMesh) 
	: Entity (uID)
{
	mesh = newMesh;
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CRenderableObject::~CRenderableObject()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}
