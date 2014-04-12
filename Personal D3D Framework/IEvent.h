#pragma once
#include "Common.h"

class IEvent
{
public:
	IEvent(std::string initType);
	~IEvent();
	void* eData;
	std::string eType;
};

