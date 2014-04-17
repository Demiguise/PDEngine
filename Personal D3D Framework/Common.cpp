#include "Common.h"

//======== Engine Vector2 (Aka, 2 dimensional vector)

EnVector2::EnVector2()
	: x(0), y(0){}

EnVector2::EnVector2(const float& initX, const float& initY)
	: x(initX), y(initY){}

EnVector2 EnVector2::operator+ (const EnVector2& rhs)
{
	EnVector2 newVect;
	newVect += rhs;
	return newVect;
}

EnVector2& EnVector2::operator+= (const EnVector2& rhs)
{
	x + rhs.x;
	y + rhs.y;
	return *this;
}

EnVector2 EnVector2::operator- (const EnVector2& rhs)
{
	EnVector2 newVect;
	newVect -= rhs;
	return newVect;
}

EnVector2& EnVector2::operator-= (const EnVector2& rhs)
{
	x - rhs.x;
	y - rhs.y;
	return *this;
}

EnVector2 EnVector2::Normalized()
{
	float mag = GetMagnitude();
	return EnVector2((x/mag), (y/mag));
}

float EnVector2::GetMagnitude()
{
	float a = pow(x, 2);
	float b = pow(y, 2);
	return sqrt(a+b);
}

//======== Engine Vector3 (Aka, 3 dimensional vector)

EnVector3::EnVector3()
	: x(0), y(0), z(0){}

EnVector3::EnVector3(const float& initX, const float& initY, const float& initZ)
	: x(initX), y(initY), z(initZ){}

EnVector3 EnVector3::operator+ (const EnVector3& rhs)
{
	EnVector3 newVect;
	newVect += rhs;
	return newVect;
}

EnVector3& EnVector3::operator+= (const EnVector3& rhs)
{
	x + rhs.x;
	y + rhs.y;
	z + rhs.z;
	return *this;
}

EnVector3 EnVector3::operator- (const EnVector3& rhs)
{
	EnVector3 newVect;
	newVect -= rhs;
	return newVect;
}

EnVector3& EnVector3::operator-= (const EnVector3& rhs)
{
	x - rhs.x;
	y - rhs.y;
	z +-rhs.z;
	return *this;
}
EnVector3 EnVector3::Normalized()
{
	float mag = GetMagnitude();
	return EnVector3((x/mag), (y/mag), (z/mag));
}

float EnVector3::GetMagnitude()
{
	float a = pow(x, 2);
	float b = pow(y, 2);
	float c = pow(z, 2);
	return sqrt(a+b+c);
}

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