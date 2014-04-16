#pragma once
#include "Common.h"

class IEvent
{
public:
	IEvent();
	~IEvent();
	void* eData;
	UINT pDataSize;
	std::string eType;
};

