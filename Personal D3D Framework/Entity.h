#pragma once
#include "Common.h"
#include "RigidBody.h"
#include "IEventManager.h"
#include "IEvent.h"

class IEventManager;

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

	UINT uID;
	EnVector3 position;
	EnVector3 rotation;
	EnVector3 resultantForce;
	CRigidBody rigidBody;

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
};

