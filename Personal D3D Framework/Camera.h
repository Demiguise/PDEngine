#pragma once
#include "Entity.h"
#include "Common.h"
#include "IEventManager.h"
#include "GameLog.h"

class Camera :
	public Entity
{
public:
	Camera(UINT uID, EnVector3 initPos, EnVector3 initRot);
	Camera(UINT uID, EnVector3 initPos);
	Camera(UINT uID);
	~Camera();
	void Update();
	void SetPosition(EnVector3 newPos);
	void SetRotation(EnVector3 newRot);
	bool OnEvent(IEvent* e);

	float mouseSensitivity;
	

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
	void GhettoMove(EnVector3 direction);
	void Init();

	EnVector2 curMousePosition;
};

