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
	bool CreateFile();

private:
	std::istringstream CleanFaceData (std::string line);
	std::vector<std::string> SplitString(std::string line, char delim);
	ModelData ConstructModelData(	std::vector<EnVector3> verts,
									std::vector<EnVector3> normals,
									std::vector<EnVector2> texCoords,
									std::vector<UINT> faces);
};

