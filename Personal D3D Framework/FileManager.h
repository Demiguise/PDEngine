#pragma once
#include "Common.h"
#include <fstream>
#include <sstream>

class FileManager
{
public:
	FileManager(void);
	~FileManager(void);
	ModelData LoadModelData(LPCSTR fileName);

private:
	ModelData ConstructModelData(	std::vector<XMFLOAT3> verts,
									std::vector<XMFLOAT3> normals,
									std::vector<XMFLOAT2> texCoords,
									std::vector<UINT> faces);
};

