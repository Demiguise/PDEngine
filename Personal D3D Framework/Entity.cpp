#include "Entity.h"

Entity::Entity(UINT ID, EnVector3 initPos, EnVector3 initRot)
{
	uID = ID;
	position = initPos;
	rotation = initRot;
	forceAccum = EnVector3();
	velocity = EnVector3();
	rigidBody = 0;
}

Entity::Entity(UINT ID, EnVector3 initPos)
{
	uID = ID;
	position = initPos;
	rotation = EnVector3();
	forceAccum = EnVector3();
	velocity = EnVector3();
	rigidBody = 0;
}

Entity::Entity(UINT ID)
{
	uID = ID;
	position = EnVector3();
	rotation = EnVector3();
	forceAccum = EnVector3();
	velocity = EnVector3();
	rigidBody = 0;
}

Entity::~Entity(void)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
	delete rigidBody;
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
	forceAccum += Util::ScalarProduct3D(direction, power);
}