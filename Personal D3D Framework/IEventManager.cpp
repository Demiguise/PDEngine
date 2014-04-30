#include "IEventManager.h"

typedef std::map<std::string, std::vector<Entity*>>::iterator listenerIT;
IEventManager* IEventManager::m_pInstance = 0;

IEventManager* IEventManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new IEventManager();
	}
	return m_pInstance;
}

IEventManager::IEventManager()
{
}


IEventManager::~IEventManager()
{
	delete m_pInstance;
}

void IEventManager::AddListener(std::string eventType, Entity* regEntity)
{
	listenerIT lisIT;
	lisIT = listenerMap.find(eventType);
	if (lisIT != listenerMap.end())
	{
		lisIT->second.push_back(regEntity);
	}
	else
	{
		listenerMap.insert(std::pair<std::string, std::vector<Entity*>>(eventType, std::vector<Entity*>(1, regEntity)));
	}
}

void IEventManager::RemoveListener(std::string eventType, Entity* regEntity)
{
	listenerIT lisIT;
	lisIT = listenerMap.find(eventType);
	if (lisIT != listenerMap.end())
	{
		for (int i = 0 ; lisIT->second.size() ; ++i)
		{
			if (lisIT->second[i] == regEntity)
			{
				lisIT->second.erase(lisIT->second.begin() + i);
			}
		}
	}
}

void IEventManager::RemoveAllListenersFromEnt(Entity* regEntity)
{
	//
}

void IEventManager::QueueEvent(IEvent* newEvent)
{
	mainEventQueue.push(newEvent);
}

void IEventManager::Update()
{		
	//Retrieve all registered listeners for the event type.
	//Pass event onto all of them. If ANYONE comes back with 'True' the event is consumed and no longer passed on. Breaks from loop and continues on to next statement.
	//Once complete, DELETE the event from both the queue and memory.
	while (!mainEventQueue.empty())
	{
		listenerIT lisIT;
		lisIT = listenerMap.find(mainEventQueue.front()->eType);
		if (lisIT != listenerMap.end())
		{
			for (UINT i = 0 ; i < lisIT->second.size() ; ++i)
			{
				if (lisIT->second[i]->OnEvent(mainEventQueue.front()))
				{
					break;
				}
			}
		}
		else
		{
			//No-one is listening for that event.
			GameLog::Log("[EventManager] No Listeners found for event.", DebugLevel::Warning);
		}
		delete mainEventQueue.front();
		mainEventQueue.pop();
	}
}