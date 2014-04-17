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
	return false;
}

void Entity::AddForce(EnVector3 direction, float power)
{
	resultantForce += Util::ScalarProduct3D(direction, power);
}