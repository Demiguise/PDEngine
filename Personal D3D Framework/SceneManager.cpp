#include "SceneManager.h"

SceneManager::SceneManager(FileManager* fileManager)
{
	mFileManager = fileManager;
}


SceneManager::~SceneManager()
{

}

void SceneManager::RegisterEntity(Entity* entity)
{
	availableEntities.push_back(entity);
}

void SceneManager::RemoveEntity(Entity* entity)
{
	for (UINT i = 0 ; i < availableEntities.size() ; ++i)
	{
		if (availableEntities[i] == entity)
		{
			availableEntities.erase(availableEntities.begin() + i);
		}
	}
}

void SceneManager::UpdateEntities()
{
	for (UINT i = 0 ; i < availableEntities.size() ; ++i)
	{
		availableEntities[i]->Update();
	}
}

UINT SceneManager::GenerateUID()
{
	UINT newID = 0;
	for (UINT i = 0 ; i < availableEntities.size() ; ++i)
	{
		if (availableEntities[i]->uID >= newID)
		{
			newID = availableEntities[i]->uID + 1;
		}
	}
	return newID;
}

void SceneManager::SetActiveCamera(Camera* newCam)
{
	activeCamera = newCam;
}