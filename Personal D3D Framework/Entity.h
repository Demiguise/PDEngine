#pragma once
#include "Common.h"
#include "IEventManager.h"
#include "IEvent.h"
#include <xnamath.h>

class IEventManager;

class Entity
{
public:
	Entity(UINT uID, XMFLOAT3 initPos, XMFLOAT3 initRot);
	Entity(UINT uID, XMFLOAT3 initPos);
	Entity(UINT ID);
	~Entity(void);
	void Update(float dt);
	bool OnEvent(IEvent* e);

	UINT uID;
	XMFLOAT3 position;
	XMFLOAT3 rotation;

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
};

