#include "Entity.h"


Entity::Entity(UINT ID)
	: uID(ID)
{
	renderable = true;
	position = XMFLOAT3(0,0,0);
	//Testing events.
	IEventManager* eManager = IEventManager::getInstance();
	eManager->AddListener("TestEvent", this);
}


Entity::~Entity(void)
{

}

void Entity::InitMeshData(ModelData* newMesh)
{
	mesh.vData = newMesh->vData;
	mesh.iData = newMesh->iData;
}

//Runtime

void Entity::Update(float dt)
{
	
}

bool Entity::OnEvent(IEvent* e)
{
	if (e->eType == "TestEvent")
	{
		OutputDebugString(L"Consuming Test Event!\n");
		return true;
	}
	return false;
}