#pragma once
#include "Common.h"

class IEvent
{
public:
	IEvent();
	~IEvent();
	void* eData;
	std::string eType;
};

