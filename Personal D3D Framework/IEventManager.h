#pragma once
#include "Common.h"
#include "IEvent.h"
#include "Entity.h"

//Singleton Design Pattern
class Entity;

class IEventManager
{
public:
	static IEventManager* getInstance();

	~IEventManager();
	void AddListener(std::string eventType, Entity* regEntity);
	void RemoveListener(std::string eventType, Entity* regEntity);
	void QueueEvent(IEvent* newEvent);
	void Update();
private:
	IEventManager();
	static IEventManager* m_pInstance;
	std::queue<IEvent*, std::deque<IEvent*>> mainEventQueue;
	std::map<std::string, std::vector<Entity*>> listenerMap;
};
