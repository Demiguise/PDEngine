#pragma once
#include "Common.h"

class Camera
{
public:
	Camera(XMFLOAT3 initPos, XMFLOAT3 initRot);
	Camera(XMFLOAT3 initPos);
	Camera();
	~Camera(void);
	void SetPosition(XMFLOAT3 newPos);
	void SetRotation(XMFLOAT3 newRot);
	XMMATRIX GetViewMatrix();

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;

};

