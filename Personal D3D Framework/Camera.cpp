#include "Camera.h"

Camera::Camera(UINT uID, EnVector3 initPos, EnVector3 initRot) 
	: Entity (uID, initPos, initRot)
{
	position = initPos;
	rotation = initRot;
	Init();
}

Camera::Camera(UINT uID, EnVector3 initPos) 
	: Entity (uID, initPos)
{
	position = initPos;
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
	Init();
}

Camera::Camera(UINT uID) 
	: Entity (uID)
{
	position = EnVector3(0.0f, 0.0f, 0.0f);
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
	Init();
}

Camera::~Camera()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}

void Camera::Init()
{
	mouseSensitivity = 0.45f;
	curMousePosition = EnVector2(400.0f, 300.0f);
	AddListeners();
}


void Camera::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("UserMouseMove", this);
	eventMan->AddListener("UserKeyPress", this);
	eventMan->AddListener("UserKeyRelease", this);
}

void Camera::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, this);
}

void Camera::Update()
{

	UpdateQuaternion();
	UpdateLocalToWorldMatrix();
	//Update our AABB to our current co-ordinates.
	if (rigidBody != 0)
	{
		rigidBody->ReCalculateAABB(AABB);
	}
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
		InputEvent* keyPress = static_cast<InputEvent*>(e);
		switch (keyPress->keyEvent)
		{
		case GameKey::W:
			GhettoMove(EnVector3(0.0f, 0.0f, -1.0f));
			break;
		case GameKey::A:
			GhettoMove(EnVector3(1.0f, 0.0f, 0.0f));
			break;
		case GameKey::S:
			GhettoMove(EnVector3(0.0f, 0.0f, 1.0f));
			break;
		case GameKey::D:
			GhettoMove(EnVector3(-1.0f, 0.0f, 0.0f));
			break;
		}
		return true;
	}
	if (e->eType == "UserMouseMove")
	{
		InputEvent* mouseMove = static_cast<InputEvent*>(e);
		EnVector2 localMove = mouseMove->mouseMovement;
		float deltaX = curMousePosition.x - mouseMove->mouseMovement.x;
		float deltaY = curMousePosition.y - mouseMove->mouseMovement.y;
		rotation += EnVector3((deltaY * mouseSensitivity), (deltaX * mouseSensitivity), 0.0f);
		GameLog::GetInstance()->Log(DebugChannel::Events, DebugLevel::Normal, "[Events] Mouse cursor has moved by (%f, %f)", deltaX, deltaY);
		curMousePosition = localMove;
	}

	return false;
}

void Camera::GhettoMove(EnVector3 direction)
{
	EnVector3 newPosition = position + direction;
	newPosition.x = position.x + GetLocalAxis(0).ADot(direction);
	newPosition.y = position.y + GetLocalAxis(1).ADot(direction);
	newPosition.z = position.z + GetLocalAxis(2).ADot(direction);
	return SetPosition(newPosition);
}
