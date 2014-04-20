#pragma once
#include "Entity.h"

class CRenderableObject :
	public Entity
{
public:
	CRenderableObject(UINT uID, ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	CRenderableObject(UINT uID, ModelData newMesh, EnVector3 initPos);
	CRenderableObject(UINT uID, ModelData newMesh);
	~CRenderableObject();
	
	void Update();
	bool renderable;
	ModelData mesh;
};

