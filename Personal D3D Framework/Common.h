#pragma once
#include <Windows.h>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <string>
#include <queue>
#include <cmath>
#include <assert.h>

//Forward Declarations
class EnVector2;
class EnVector3;
class EnMatrix2x2;
class EnMatrix3x3;

//Common Classes

class EnVector2
{
public:
	EnVector2();
	EnVector2(const float& initX, const float& initY);
	~EnVector2();
	static EnVector2 Zero();
	EnVector2 Normalized();
	float GetMagnitude() const;
	EnVector2& operator+= (const EnVector2& rhs);
	EnVector2& operator-= (const EnVector2& rhs);
	
	float x;
	float y;

};
EnVector2 operator+ (EnVector2 lhs, const EnVector2& rhs);
EnVector2 operator- (EnVector2 lhs, const EnVector2& rhs);

class EnVector3
{
public:
	EnVector3();
	EnVector3(const float& initX, const float& initY, const float& initZ);
	~EnVector3();
	static EnVector3 Zero();
	EnVector3 Dot(const EnVector3& rhs);
	EnVector3 Cross(const EnVector3& rhs);
	EnVector3 MatrixMult3x3(const EnMatrix3x3& rhs);
	EnVector3 Normalized();
	float GetMagnitude() const;
	EnVector3& operator+= (const EnVector3& rhs);
	EnVector3& operator-= (const EnVector3& rhs);
	float& operator[] (int index);

	float x;
	float y;
	float z;
};
EnVector3 operator+ (EnVector3 lhs, const EnVector3& rhs);
EnVector3 operator- (EnVector3 lhs, const EnVector3& rhs);

class EnMatrix2x2
{
public:
	EnMatrix2x2();
	EnMatrix2x2(EnVector2 r1, EnVector2 r2);
	~EnMatrix2x2();
	EnVector2 r[2];
	float GetDeterminant();
};

class EnMatrix3x3
{
public:
	EnMatrix3x3();
	EnMatrix3x3(EnVector3 r1, EnVector3 r2, EnVector3 r3);
	~EnMatrix3x3();
	EnVector3 r[3];
	EnMatrix2x2 CreateMinor(const int& row, const int& column);
	bool Invert();
	float GetDeterminant();
};

//Common Structs

struct Vertex
{
	Vertex(){}
	Vertex(const EnVector3& p, const EnVector3& n, const EnVector2& tc)
		: position(p), normal(n), texCoords(tc){}
	EnVector3 position;
	EnVector3 normal;
	EnVector2 texCoords;
};

struct BoundingBox
{
	BoundingBox() { minPoint = EnVector3();
					maxPoint = EnVector3(); }
	EnVector3 minPoint;
	EnVector3 maxPoint;
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

//Common Enums

enum GameKey
{
	W,
	A,
	S,
	D
};

//Common Namespaces

//Utils namespace - Contains some needed functions for stuff. Yeah.
namespace Util
{
	EnVector2 ScalarProduct2D(const EnVector2& v, const float& s);
	EnVector3 ScalarProduct3D(const EnVector3& v, const float& s);
	EnMatrix3x3 ScalarProduct3x3(const EnMatrix3x3&m, const float& s);

	template<class T> void SwapValues(T& a, T& b);

};


