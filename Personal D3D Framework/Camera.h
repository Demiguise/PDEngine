#pragma once
#include "Entity.h"
#include "Common.h"
#include "IEvent.h"
#include "IEventManager.h"

class Camera :
	public Entity
{
public:
	Camera(UINT uID, EnVector3 initPos, EnVector3 initRot);
	Camera(UINT uID, EnVector3 initPos);
	Camera(UINT uID);
	~Camera();
	void SetPosition(EnVector3 newPos);
	void SetRotation(EnVector3 newRot);
	bool OnEvent(IEvent* e);

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
	void GhettoMove(EnVector3 direction);
};

