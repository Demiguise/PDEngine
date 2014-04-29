#include "Common.h"

//======== Engine Vector2 (Aka, 2 dimensional vector)
EnVector2::EnVector2()
	: x(0), y(0){}

EnVector2::EnVector2(const float& initX, const float& initY)
	: x(initX), y(initY){}

EnVector2::~EnVector2()
{
}

EnVector2 EnVector2::Normalized()
{
	float mag = GetMagnitude();
	if (mag == 0) { return EnVector2(); }
	return EnVector2((x/mag), (y/mag));
}

float EnVector2::GetMagnitude()
{
	float a = x * x;
	float b = y * x;
	return sqrt(a+b);
}

//Member operators
EnVector2& EnVector2::operator+= (const EnVector2& rhs)
{
	x = x + rhs.x;
	y = y + rhs.y;
	return *this;
}

EnVector2& EnVector2::operator-= (const EnVector2& rhs)
{
	x = x - rhs.x;
	y = y - rhs.y;
	return *this;
}

//Non-Member operators
EnVector2 operator+ (EnVector2 lhs, const EnVector2& rhs)
{
	lhs += rhs;
	return lhs;
}

EnVector2 operator- (EnVector2 lhs, const EnVector2& rhs)
{
	lhs -= rhs;
	return lhs;
}

//======== Engine Vector3 (Aka, 3 dimensional vector)
EnVector3::EnVector3()
	: x(0), y(0), z(0){}

EnVector3::EnVector3(const float& initX, const float& initY, const float& initZ)
	: x(initX), y(initY), z(initZ){}

EnVector3::~EnVector3()
{
}

EnVector3 EnVector3::Normalized()
{
	float mag = GetMagnitude();
	if (mag == 0) { return EnVector3(); }
	return EnVector3((x/mag), (y/mag), (z/mag));
}

float EnVector3::GetMagnitude()
{
	float a = x * x;
	float b = y * y;
	float c = z * z;
	return sqrt(a+b+c);
}

//Member operators
EnVector3& EnVector3::operator+= (const EnVector3& rhs)
{
	x = x + rhs.x;
	y = y + rhs.y;
	z = z + rhs.z;
	return *this;
}

EnVector3& EnVector3::operator-= (const EnVector3& rhs)
{
	x = x - rhs.x;
	y = y - rhs.y;
	z = z - rhs.z;
	return *this;
}

//Non-member operators
EnVector3 operator- (EnVector3 lhs, const EnVector3& rhs)
{
	lhs -= rhs;
	return lhs;
}

EnVector3 operator+ (EnVector3 lhs, const EnVector3& rhs)
{
	lhs += rhs;
	return lhs;
}

//======== Engine Matrix3x3
EnMatrix3x3::EnMatrix3x3()
{
}

EnMatrix3x3::EnMatrix3x3(EnVector3 r1, EnVector3 r2, EnVector3 r3)
{
	r[0] = r1;
	r[1] = r2;
	r[2] = r3;
}

EnMatrix3x3::~EnMatrix3x3()
{
}

bool EnMatrix3x3::Invert()
{
	EnMatrix3x3 backup(r[0], r[1], r[2]);

	return true;
}

//Utilities that I might find useful
namespace Util
{
	EnVector2 ScalarProduct2D(const EnVector2& v, const float& s)
	{
		float x = v.x * s;
		float y = v.y * s;
		return EnVector2(x, y);
	}

	EnVector3 ScalarProduct3D(const EnVector3& v, const float& s)
	{
		float x = v.x * s;
		float y = v.y * s;
		float z = v.z * s;
		return EnVector3(x, y, z);
	}
}

namespace GameLog
{
	bool GameLog::InitLog()
	{
		OutputDebugString(L"Initialising Log!\n");
		return true;
	}

	void GameLog::Log(const char* message, const DebugLevel logLevel)
	{
	}
}