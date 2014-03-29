#pragma once
#include "Common.h"
#include <xnamath.h>

class Entity
{
public:
	Entity(UINT ID);
	~Entity(void);
	void InitMeshData(ModelData* newMesh);
	void Update(float dt);

	UINT uID;
	ModelData mesh;
	bool renderable;
	XMFLOAT3 position;

private:


};

