#include "SceneManager.h"

SceneManager::SceneManager(FileManager* fileManager, Renderer* renderer)
{
	mFileManager = fileManager;
	mRenderer = renderer;
}


SceneManager::~SceneManager()
{

}

Entity* SceneManager::CreateEntity(LPCSTR EntityName, LPCSTR meshFileName)
{
	availableEntities.push_back(Entity(GenerateUID()));
	Entity* newEntity = &availableEntities.back();
	ModelData newMesh;
	std::map<LPCSTR, ModelData>::iterator it = MeshReference.find(meshFileName);
	if (it != MeshReference.end())
	{
		newMesh = MeshReference[meshFileName];
	}
	else
	{
		newMesh = mFileManager->LoadModelData(meshFileName);
		MeshReference.insert(std::pair<LPCSTR, ModelData>(meshFileName, newMesh));
	}
	newEntity->InitMeshData(&newMesh);
	mRenderer->CreateBuffer(newEntity);
	return newEntity;
}

void SceneManager::DestroyEntity(UINT EntityUID)
{
	
}

UINT SceneManager::GenerateUID()
{
	UINT newID = 0;
	for (UINT i = 0 ; i < availableEntities.size() ; i++)
	{
		if (availableEntities[i].uID >= newID)
		{
			newID = availableEntities[i].uID + 1;
		}
	}
	return newID;
}

std::vector<Entity> SceneManager::FindSceneObjects()
{
	std::vector<Entity> renderableObjects;
	for (UINT i = 0 ; i < availableEntities.size() ; i++)
	{
		if(availableEntities[i].renderable)
		{
			renderableObjects.push_back(availableEntities[i]);
	
		}
	}
	if (prevSceneEntityCount != availableEntities.size())
	{
		sceneChangeThisFrame = true;
		prevSceneEntityCount = availableEntities.size();
	}
	else
	{
		sceneChangeThisFrame = false;
	}
	return renderableObjects;
}

void SceneManager::SetActiveCamera(Camera newCam)
{
	activeCamera = newCam;
}