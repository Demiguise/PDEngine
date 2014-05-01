#pragma once
#include "Entity.h"
#include "RenderableObject.h"
#include "Renderer.h"
#include "FileManager.h"
#include "Camera.h"
#include "GameLog.h"

class SceneManager
{
public:
	SceneManager(FileManager* fileManager);
	~SceneManager();

	void RegisterEntity(Entity* entity);
	void RemoveEntity(Entity* entity);
	UINT GenerateUID();
	void SetActiveCamera(Camera* newCam);
	void UpdateEntities();

	Camera* activeCamera;
	bool sceneChangeThisFrame;
	
private:
	ModelData InitModelData(LPSTR meshFileName);

	std::deque<Entity*> availableEntities;
	UINT prevSceneEntityCount;
	std::map<LPCSTR, ModelData> MeshReference;
	FileManager* mFileManager;
};

