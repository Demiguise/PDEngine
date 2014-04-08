#pragma once
#include "Common.h"
#include <queue>

class EventManager
{
public:
	EventManager();
	~EventManager();

private:
	std::queue<int, std::deque<int>> mainEventQueue;
};

