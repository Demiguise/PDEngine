#pragma once
#include "entity.h"

class CRenderableObject :
	public Entity
{
public:
	CRenderableObject(UINT uID, ModelData newMesh, XMFLOAT3 initPos, XMFLOAT3 initRot);
	CRenderableObject(UINT uID, ModelData newMesh, XMFLOAT3 initPos);
	CRenderableObject(UINT uID, ModelData newMesh);
	~CRenderableObject();
	
	bool renderable;
	ModelData mesh;
};

