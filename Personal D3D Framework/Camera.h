#pragma once
#include "Entity.h"
#include "Common.h"
#include "IEvent.h"
#include "IEventManager.h"

class Camera :
	public Entity
{
public:
	Camera(UINT uID, XMFLOAT3 initPos, XMFLOAT3 initRot);
	Camera(UINT uID, XMFLOAT3 initPos);
	Camera(UINT uID);
	~Camera();
	void SetPosition(XMFLOAT3 newPos);
	void SetRotation(XMFLOAT3 newRot);
	XMMATRIX GetViewMatrix();
	bool OnEvent(IEvent* e);

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
	void GhettoMove(XMFLOAT3 direction);
};

