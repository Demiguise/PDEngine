#pragma once
#include "Common.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <direct.h>

const char logLocation[] = "Game.log";
const char backupLocation[] = "./Logs/";

class FileManager
{
public:
	static FileManager* GetInstance();
	~FileManager();
	ModelData LoadModelData(LPCSTR fileName);
	void WriteToLog(const char* message);

private:
	FileManager();
	static FileManager* m_pInstance;
	std::istringstream CleanFaceData (std::string line);
	std::vector<std::string> SplitString(std::string line, char delim);
	ModelData ConstructModelData(	std::vector<EnVector3> verts,
									std::vector<EnVector3> normals,
									std::vector<EnVector2> texCoords,
									std::vector<UINT> faces);
	std::fstream logStream;
	void GetLocalTime(char* buffer);
	void MoveAndRenameLog();
};

