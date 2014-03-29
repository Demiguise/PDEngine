#include "FileManager.h"


FileManager::FileManager(void)
{

}


FileManager::~FileManager(void)
{

}

ModelData FileManager::LoadModelData(LPCSTR fileName)
{
	std::ifstream ifs (fileName, std::ios_base::in);
	if (ifs.is_open() == NULL)
	{
		return ModelData();
	}
	std::string line;
	std::vector<XMFLOAT3> lVertices;
	std::vector<XMFLOAT3> lNormals;
	std::vector<XMFLOAT2> lTexCoords;
	std::vector<UINT> lFaces;
	OutputDebugString(L"Loading data from file.\n");
	while (std::getline(ifs, line))
	{
		if(line.substr(0, 2) == "v ")
		{
			std::istringstream iss(line.substr(2));
			XMFLOAT3 nV;
			iss >> nV.x; iss >> nV.y; iss >> nV.z;
			lVertices.push_back(nV);
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream iss(line.substr(3));
			XMFLOAT2 nVT;
			iss >> nVT.x; iss >> nVT.y;
			lTexCoords.push_back(nVT);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream iss(line.substr(3));
			XMFLOAT3 nVN;
			iss >> nVN.x; iss >> nVN.y; iss >> nVN.z;
			lNormals.push_back(nVN);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream iss(line.substr(2));
			UINT tA, tB, tC;
			iss >> tA; iss >> tB, iss >> tC;
			lFaces.push_back(tA); lFaces.push_back(tB); lFaces.push_back(tC);
		}
		else
		{
			//Ignore any other lines
		}
	}
	OutputDebugString(L"Finished reading data from file.\n");
	return ConstructModelData(lVertices, lNormals, lTexCoords, lFaces);
}

ModelData FileManager::ConstructModelData(	std::vector<XMFLOAT3> verts,
											std::vector<XMFLOAT3> normals,
											std::vector<XMFLOAT2> texCoords,
											std::vector<UINT> faces)
{
	ModelData newMesh;
	for (UINT i = 0 ; i < verts.size() ; i++)
	{
		newMesh.vData.push_back(Vertex(verts[i], normals[i], texCoords[i]));
	}
	newMesh.iData = faces;
	return newMesh;
}
