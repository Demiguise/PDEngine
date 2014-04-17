#include "Camera.h"

Camera::Camera(UINT uID, EnVector3 initPos, EnVector3 initRot) 
	: Entity (uID, initPos, initRot)
{
	position = initPos;
	rotation = initRot;
	AddListeners();
}

Camera::Camera(UINT uID, EnVector3 initPos) 
	: Entity (uID, initPos)
{
	position = initPos;
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
	AddListeners();
}

Camera::Camera(UINT uID) 
	: Entity (uID)
{
	position = EnVector3(0.0f, 0.0f, 0.0f);
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
	AddListeners();
}

Camera::~Camera()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}

void Camera::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("UserKeyPress", this);
	eventMan->AddListener("UserKeyRelease", this);
}

void Camera::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, this);
}

void Camera::SetPosition(EnVector3 newPos)
{
	position = newPos;
}

void Camera::SetRotation(EnVector3 newRot)
{
	rotation = newRot;
}

bool Camera::OnEvent(IEvent* e)
{
	if (e->eType == "UserKeyPress")
	{
		GameKey* keyPress = static_cast<GameKey*>(e->eData);
		switch (*keyPress)
		{
		case GameKey::W:
			GhettoMove(EnVector3(1.0f, 0.0f, 0.0f));
			break;
		case GameKey::A:
			GhettoMove(EnVector3(0.0f, 1.0f, 0.0f));
			break;
		case GameKey::S:
			GhettoMove(EnVector3(-1.0f, 0.0f, 0.0f));
			break;
		case GameKey::D:
			GhettoMove(EnVector3(0.0f, -1.0f, 0.0f));
			break;
		}
		return true;
	}
	return false;
}

void Camera::GhettoMove(EnVector3 direction)
{
	EnVector3 newPosition;
	newPosition.x = position.x + direction.x;
	newPosition.y = position.y + direction.y;
	newPosition.z = position.z + direction.z;
	return SetPosition(newPosition);
}
