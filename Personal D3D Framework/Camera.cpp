#include "Camera.h"

Camera::Camera(XMFLOAT3 initPos, XMFLOAT3 initRot)
{
	position = initPos;
	rotation = initRot;
}

Camera::Camera(XMFLOAT3 initPos)
{
	position = initPos;
}

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Camera::~Camera(void)
{
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