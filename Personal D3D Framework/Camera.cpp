#include "Camera.h"

Camera::Camera(UINT uID, XMFLOAT3 initPos, XMFLOAT3 initRot) 
	: Entity (uID, initPos, initRot)
{
	position = initPos;
	rotation = initRot;
	AddListeners();
}

Camera::Camera(UINT uID, XMFLOAT3 initPos) 
	: Entity (uID, initPos)
{
	position = initPos;
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	AddListeners();
}

Camera::Camera(UINT uID) 
	: Entity (uID)
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
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

void Camera::SetPosition(XMFLOAT3 newPos)
{
	position = newPos;
}

void Camera::SetRotation(XMFLOAT3 newRot)
{
	rotation = newRot;
}

XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR camPosVector = XMVectorSet(position.x, position.y, position.z, 1.0f);
	return XMMatrixLookAtLH(camPosVector, XMVectorZero(), up);
}

bool Camera::OnEvent(IEvent* e)
{
	if (e->eType == "UserKeyPress")
	{
		GameKey* keyPress = static_cast<GameKey*>(e->eData);
		switch (*keyPress)
		{
		case GameKey::W:
			GhettoMove(XMFLOAT3(1.0f, 0.0f, 0.0f));
			break;
		case GameKey::A:
			GhettoMove(XMFLOAT3(0.0f, 1.0f, 0.0f));
			break;
		case GameKey::S:
			GhettoMove(XMFLOAT3(-1.0f, 0.0f, 0.0f));
			break;
		case GameKey::D:
			GhettoMove(XMFLOAT3(0.0f, -1.0f, 0.0f));
			break;
		}
		return true;
	}
	return false;
}

void Camera::GhettoMove(XMFLOAT3 direction)
{
	XMFLOAT3 newPosition;
	newPosition.x = position.x + direction.x;
	newPosition.y = position.y + direction.y;
	newPosition.z = position.z + direction.z;
	return SetPosition(newPosition);
}
