#include "Entity.h"

Entity::Entity(UINT ID, EnVector3 initPos, EnVector3 initRot)
{
	uID = ID;
	position = initPos;
	rotation = initRot;
}

Entity::Entity(UINT ID, EnVector3 initPos)
{
	uID = ID;
	position = initPos;
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
}

Entity::Entity(UINT ID)
{
	uID = ID;
	position = EnVector3(0,0,0);
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
	//Testing events.
	IEventManager* eManager = IEventManager::GetInstance();
	eManager->AddListener("TestEvent", this);
}

Entity::~Entity(void)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}

//Runtime

void Entity::Update()
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

void Entity::AddForce(EnVector3 direction, int power)
{
	
}