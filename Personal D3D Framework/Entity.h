#pragma once
#include "Common.h"
#include "Colliders.h"
#include "IEventManager.h"
#include "IEvent.h"

class IEventManager;
class CRigidBody;

class Entity
{
public:
	Entity(UINT uID, EnVector3 initPos, EnVector3 initRot);
	Entity(UINT uID, EnVector3 initPos);
	Entity(UINT ID);
	~Entity(void);
	virtual void Update();
	virtual bool OnEvent(IEvent* e);
	void AddForce(EnVector3 direction, float power);
	void SetRigidBody(UINT typeFlag, UINT initMass, float scale);

	UINT uID;
	EnVector3 position;
	EnVector3 rotation;
	EnVector3 forceAccum;
	EnVector3 velocity;
	CRigidBody* rigidBody;

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
};

