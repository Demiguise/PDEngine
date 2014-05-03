#include "FileManager.h"

FileManager* FileManager::m_pInstance = 0;

FileManager* FileManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new FileManager();
	}
	return m_pInstance;
}

FileManager::FileManager()
{
	//GameLog::GetInstance()->Log("[FileManager] Initialisation Complete.", DebugLevel::Normal);
	//Test if a log file already exists.
	std::ifstream logFile(logLocation);
	if (logFile)
	{
		logFile.close();
		MoveAndRenameLog();
		OutputDebugString(L"Log renamed!");
	}
}

FileManager::~FileManager()
{
	delete m_pInstance;
}

ModelData FileManager::LoadModelData(LPCSTR fileName)
{
	std::ifstream ifs (fileName, std::ios_base::in);
	if (ifs.is_open() == NULL)
	{
		return ModelData();
	}
	std::string line;
	std::vector<EnVector3> lVertices;
	std::vector<EnVector3> lNormals;
	std::vector<EnVector2> lTexCoords;
	std::vector<UINT> lFaces;
	//OutputDebugString(L"Loading data from file.\n");
	while (std::getline(ifs, line))
	{
		if(line.substr(0, 2) == "v ")
		{
			std::istringstream iss(line.substr(2));
			EnVector3 nV;
			iss >> nV.x; iss >> nV.y; iss >> nV.z;
			lVertices.push_back(nV);
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream iss(line.substr(3));
			EnVector2 nVT;
			iss >> nVT.x; iss >> nVT.y;
			lTexCoords.push_back(nVT);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream iss(line.substr(3));
			EnVector3 nVN;
			iss >> nVN.x; iss >> nVN.y; iss >> nVN.z;
			lNormals.push_back(nVN);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream iss(CleanFaceData(line.substr(2)));
			UINT tA, tB, tC;
			iss >> tA; iss >> tB, iss >> tC;
			lFaces.push_back(tA - 1); lFaces.push_back(tB - 1); lFaces.push_back(tC - 1); //I'm taking away one here because the .obj files I'm using don't start from 0, rather they start from 1.
		}
		else
		{
			//Ignore any other lines
		}
	}
	//OutputDebugString(L"Finished reading data from file.\n");
	ModelData newModel = ConstructModelData(lVertices, lNormals, lTexCoords, lFaces);
	newModel.semanticName = fileName;
	return newModel;
}

std::istringstream FileManager::CleanFaceData(std::string line)
{
	std::istringstream newStream;
	std::string finishedString;
	std::vector<std::string> splitLine = SplitString(line, ' ');
	for (UINT i = 0 ; i < splitLine.size() ; ++i)
	{
		std::vector<std::string> splitFaces = SplitString(splitLine[i], '/');
		finishedString.append(splitFaces[0]);
		finishedString.append(" ");
	}
	newStream.str(finishedString);
	return newStream;
}

std::vector<std::string> FileManager::SplitString(std::string line, char delim)
	{
		char curChar;
		std::vector<std::string> stringVector;
		std::string curString;
		for (UINT i = 0 ; i < line.size() ; ++i)
		{
			curChar = line[i];
			if (curChar == delim)
			{
				stringVector.push_back(curString);
				curString = "";
			}
			else
			{
				curString.push_back(curChar);
			}
		}
		stringVector.push_back(curString);
		return stringVector;
	}

ModelData FileManager::ConstructModelData(	std::vector<EnVector3> verts,
											std::vector<EnVector3> normals,
											std::vector<EnVector2> texCoords,
											std::vector<UINT> faces)
{
	ModelData newMesh;
	if (normals.size() == 0)
	{
		normals = std::vector<EnVector3>(verts.size(), EnVector3(0.0f, 0.0f, 0.0f));
	}
	if (texCoords.size() == 0)
	{
		texCoords = std::vector<EnVector2>(verts.size(), EnVector2(0.0f, 0.0f));
	}
	for (UINT i = 0 ; i < verts.size() ; ++i)
	{
		newMesh.vData.push_back(Vertex(verts[i], normals[i], texCoords[i]));
	}
	newMesh.iData = faces;
	return newMesh;
}

void FileManager::WriteToLog(const char* message)
{
	logStream.open(logLocation, std::fstream::trunc | std::fstream::out);
	if (logStream.fail())
	{
		logStream = std::fstream(logLocation, std::fstream::trunc | std::fstream::out);
		//Make new file
	}
	char timeBuf[80];
	GetLocalTime(timeBuf);
	logStream << '<' << timeBuf << '>';
	logStream.write(message, strlen(message));
	logStream.close();
}

void FileManager::GetLocalTime(char* buffer)
{
	std::time_t rawTime;
	struct tm* timeInfo;
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	strftime(buffer, 80, "%d-%m %H-%M-%S", timeInfo);
}

void FileManager::MoveAndRenameLog()
{
	if (_mkdir(backupLocation) != 0)
	{
		OutputDebugString(L"Failed to create the backup log location");
	}
	char timeBuf[80];
	GetLocalTime(timeBuf);
	std::string newLogLocation = backupLocation;
	newLogLocation.append("(");
	newLogLocation.append(timeBuf);
	newLogLocation.append(")");
	newLogLocation.append(" Game.log");
	std::ofstream destFile(newLogLocation);
	if (!destFile)
	{
		OutputDebugString(L"Couldn't create new outputfile");
	}
	std::ifstream originFile(logLocation);
	destFile << originFile.rdbuf();
	originFile.close();
	destFile.close();
}