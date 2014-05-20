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
class Quaternion;
class EnVector2;
class EnVector3;
class EnVector4;
class EnMatrix2x2;
class EnMatrix3x3;
class EnMatrix4x4;

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
	EnVector3 GDot(const EnVector3& rhs); //Geometric Dot product - Returns new Vector
	float ADot(const EnVector3& rhs); //Algebraic Dot product - Returns scalar value
	float FindAngleBetween(const EnVector3& rhs);
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

class EnVector4
{
public:
	EnVector4();
	EnVector4(const float& initX, const float& initY, const float& initZ, const float& initW);
	~EnVector4();
	static EnVector4 Zero();
	EnVector4 Normalized();
	float GetMagnitude() const;

	float x;
	float y;
	float z;
	float w;
};

class EnMatrix2x2
{
public:
	EnMatrix2x2();
	EnMatrix2x2(const EnVector2& c1, const EnVector2& c2);
	~EnMatrix2x2();
	EnVector2 c[2];
	static EnMatrix2x2 Identity();
	float GetDeterminant();
};

class EnMatrix3x3
{
public:
	EnMatrix3x3();
	EnMatrix3x3(const EnVector3& c1, const EnVector3& c2, const EnVector3& c3);
	~EnMatrix3x3();
	EnVector3 c[3];
	static EnMatrix3x3 Identity();
	EnMatrix2x2 CreateMinor(const int& row, const int& column);
	bool Invert();
	void Transpose();
	float GetDeterminant();
};

class EnMatrix4x4
{
public:
	EnMatrix4x4();
	EnMatrix4x4(const EnVector4& c1,const EnVector4& c2,const EnVector4& c3,const EnVector4& c4);
	~EnMatrix4x4();
	static EnMatrix4x4 Identity();
	EnVector4 c[4];
};

class Quaternion
{
public:
	Quaternion();
	Quaternion(const float& initS, const EnVector3& initVector);
	Quaternion(const float& initS, const float& initI, const float& initJ, const float& initK);
	~Quaternion();
	Quaternion Normalized();
	float scalar;
	EnVector3 vector;
	Quaternion& operator*= (const Quaternion& rhs);
};
Quaternion operator* (Quaternion lhs, const Quaternion& rhs);

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
	EnVector3 ScalarDivision3D(const EnVector3& v, const float& s);
	EnMatrix3x3 ScalarProduct3x3(const EnMatrix3x3&m, const float& s);

	template<class T> void SwapValues(T& a, T& b);

};


