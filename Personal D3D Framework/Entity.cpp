#include "Entity.h"


Entity::Entity(UINT ID)
	: uID(ID)
{
	renderable = true;
	position = XMFLOAT3(0,0,0);
}


Entity::~Entity(void)
{

}

void Entity::InitMeshData(ModelData* newMesh)
{
	mesh.vData = newMesh->vData;
	mesh.iData = newMesh->iData;
}

//Runtime

void Entity::Update(float dt)
{
	
}
