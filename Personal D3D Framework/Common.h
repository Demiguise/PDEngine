#pragma once
#include <Windows.h>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <string>
#include <queue>

struct EnVector2
{
	EnVector2(){}
	EnVector2(const float initX, const float initY)
		: x(initX), y(initY){}
	float x;
	float y;
};

struct EnVector3
{
	EnVector3(){}
	EnVector3(const float initX, const float initY, const float initZ)
		: x(initX), y(initY), z(initZ){}
	float x;
	float y;
	float z;
};

struct Vertex
{
	Vertex(){}
	Vertex(const EnVector3& p, const EnVector3& n, const EnVector2& tc)
		: position(p), normal(n), texCoords(tc){}
	EnVector3 position;
	EnVector3 normal;
	EnVector2 texCoords;
};

struct ModelData
{
	ModelData() {}
	ModelData(std::vector<Vertex> iVData, std::vector<UINT> iIData)
		: vData(iVData), iData(iIData){}
	LPCSTR semanticName;
	std::vector<Vertex> vData;
	std::vector<UINT> iData;
};

enum GameKey
{
	W,
	A,
	S,
	D
};

namespace Util
{
	
}