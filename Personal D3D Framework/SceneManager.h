#pragma once
#include "Entity.h"
#include "FileManager.h"
#include "Camera.h"


class SceneManager
{
public:
	SceneManager(FileManager* fileManager);
	~SceneManager();
	Entity CreateEntity(LPCSTR EntityName, LPCSTR meshName);
	void DestroyEntity(UINT EntityUID);
	std::vector<Entity> FindSceneObjects();
	UINT GenerateUID();
	bool sceneChangeThisFrame;
	
	std::vector<Entity> availableEntities;
	Camera activeCamera;
	void SetActiveCamera(Camera newCam);

private:
	ModelData InitModelData(LPSTR meshFileName);

	UINT prevSceneEntityCount;
	std::map<LPCSTR, ModelData> MeshReference;
	FileManager* mFileManager;
};

