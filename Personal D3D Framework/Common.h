#pragma once
#include <Windows.h>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <string>
#include <queue>

#include "Timer.h"

class EnVector2
{
public:
	EnVector2();
	EnVector2(const float& initX, const float& initY);
	~EnVector2();
	EnVector2& operator+= (const EnVector2& rhs);
	EnVector2& operator-= (const EnVector2& rhs);
	float x;
	float y;
	EnVector2 Normalized();
	float GetMagnitude();
};
EnVector2 operator+ (EnVector2 lhs, const EnVector2& rhs);
EnVector2 operator- (EnVector2 lhs, const EnVector2& rhs);

class EnVector3
{
public:
	EnVector3();
	EnVector3(const float& initX, const float& initY, const float& initZ);
	~EnVector3();
	EnVector3& operator+= (const EnVector3& rhs);
	EnVector3& operator-= (const EnVector3& rhs);
	float x;
	float y;
	float z;
	EnVector3 Normalized();
	float GetMagnitude();
};
EnVector3 operator+ (EnVector3 lhs, const EnVector3& rhs);
EnVector3 operator- (EnVector3 lhs, const EnVector3& rhs);

class EnMatrix3x3
{
public:
	EnMatrix3x3();
	EnMatrix3x3(EnVector3 r1, EnVector3 r2, EnVector3 r3);
	~EnMatrix3x3();
	EnVector3 r[3];
	bool Invert();
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
	EnVector2 ScalarProduct2D(const EnVector2& v, const float& s);
	EnVector3 ScalarProduct3D(const EnVector3& v, const float& s);
}

//0 None (Don't use for logging) | 1 Normal | 2 Errors | 3 Warnings
enum DebugLevel
{
	None = 0, 
	Normal,
	Error,
	Warning,
};

namespace GameLog
{
	bool InitLog();
	void Log(const char* message, const DebugLevel logLevel);
};

