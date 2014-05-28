#pragma once
#include "Common.h"

class IEvent
{
public:
	IEvent();
	~IEvent();
	std::string eType;
};

class InputEvent : public IEvent
{
public:
	InputEvent();
	~InputEvent();
	EnVector2 mouseMovement;
	GameKey keyEvent;
};
