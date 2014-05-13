#include "Common.h"


//======== Engine Vector2 (Aka, 2 dimensional vector)
EnVector2::EnVector2()
	: x(0), y(0){}

EnVector2::EnVector2(const float& initX, const float& initY)
	: x(initX), y(initY){}

EnVector2::~EnVector2()
{
}

EnVector2 EnVector2::Zero()
{
	return EnVector2(0.0f, 0.0f);
}

EnVector2 EnVector2::Normalized()
{
	float mag = GetMagnitude();
	if (mag == 0) { return EnVector2(); }
	return EnVector2((x/mag), (y/mag));
}

float EnVector2::GetMagnitude() const
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

EnVector3 EnVector3::Zero()
{
	return EnVector3(0.0f, 0.0f, 0.0f);
}

EnVector3 EnVector3::Cross(const EnVector3& rhs)
{
	float lhsMag = GetMagnitude();
	float rhsMag = rhs.GetMagnitude();
	float sineValue = sin(FindAngleBetween(rhs));
	EnVector3 n = EnVector3((y*rhs.z) - (z*rhs.y),
							(z*rhs.x) - (x*rhs.z),
							(x*rhs.y) - (y*rhs.x));
	return Util::ScalarProduct3D(n, (lhsMag*rhsMag*sineValue));
}

float EnVector3::FindAngleBetween(const EnVector3& rhs)
{
	float lhsMag = GetMagnitude();
	float rhsMag = rhs.GetMagnitude();
	float dotProduct = ADot(rhs);
	return acos(dotProduct/(lhsMag * rhsMag));
}

 //Geometric Dot product - Returns new Vector
EnVector3 EnVector3::GDot(const EnVector3& rhs) 
{
	float rX = x * rhs.x;
	float rY = y * rhs.y;
	float rZ = z * rhs.z;
	return EnVector3(rX, rY, rZ);
}

//Algebraic Dot product - Returns scalar value
float EnVector3::ADot(const EnVector3& rhs)
{
	return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}


EnVector3 EnVector3::MatrixMult3x3(const EnMatrix3x3& rhs)
{
	return EnVector3((rhs.r[0].x * x) + (rhs.r[0].y * y) + (rhs.r[0].z * z),
					(rhs.r[1].x * x) + (rhs.r[1].y * y) + (rhs.r[1].z * z),
					(rhs.r[2].x * x) + (rhs.r[2].y * y) + (rhs.r[2].z * z));
}

EnVector3 EnVector3::Normalized()
{
	float mag = GetMagnitude();
	if (mag == 0) { return EnVector3(); }
	return EnVector3((x/mag), (y/mag), (z/mag));
}

float EnVector3::GetMagnitude() const
{
	float a = x * x;
	float b = y * y;
	float c = z * z;
	return sqrt(a+b+c);
}

//Member operators

//Unsure if I like this being used on a vector.
//Great for matrix manipulation though.
float& EnVector3::operator[] (int index)
{
	assert ((index <= 2) && (index >= 0));
	switch (index)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	}
}

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
//	In the Form:
//	Row 0 | A B |
//	Row 1 | C D |
//	Where A = Row 0.x, B = Row 0.y, etc;
EnMatrix2x2::EnMatrix2x2()
{
}

EnMatrix2x2::EnMatrix2x2(const EnVector2& r1, const EnVector2& r2)
{
	r[0] = r1;
	r[1] = r2;
}

EnMatrix2x2::~EnMatrix2x2()
{
}

float EnMatrix2x2::GetDeterminant()
{
	return	(r[0].x * r[1].y) -
			(r[0].y * r[1].x);
}

//======== Engine Matrix3x3
//	In the Form:
//	Row 0 | A B C |
//	Row 1 | D E F |
//	Row 2 | G H I |
//	Where A = Row 0.x, B = Row 0.y, C = Row 0.z, etc.
EnMatrix3x3::EnMatrix3x3()
{
}

EnMatrix3x3::EnMatrix3x3(const EnVector3& r1, const EnVector3& r2, const EnVector3& r3)
{
	r[0] = r1;
	r[1] = r2;
	r[2] = r3;
}

EnMatrix3x3::~EnMatrix3x3()
{
}

// Will NOT overwrite the matrix if returns false. 
bool EnMatrix3x3::Invert() 
{
	EnMatrix3x3 cofactorMatrix;
	EnMatrix2x2 minorMatrix;
	float determinant = GetDeterminant();
	if (determinant == 0) { return false; }
	else
	{
		//Create new matrix containing determinants of each minor matrix.
		for (UINT i = 0 ; i < 3 ; ++i)
		{
			for (UINT j = 0 ; j < 3 ; ++j)
			{
				minorMatrix = CreateMinor(i, j);
				// +2 here as I'm working from a zero index. Can't really be doing 0+0 as a power.
				cofactorMatrix.r[i][j] = pow(-1, (i + j + 2)) * minorMatrix.GetDeterminant();
			}
		}
		//Now to flip the Matrix along the left->right diagonal.
		Util::SwapValues(cofactorMatrix.r[1][0], cofactorMatrix.r[0][1]);
		Util::SwapValues(cofactorMatrix.r[2][0], cofactorMatrix.r[0][2]);
		Util::SwapValues(cofactorMatrix.r[2][1], cofactorMatrix.r[1][2]);
	};
	//Finally, multiply the cofactor matrix by the reciprocal of the determinant
	//to get the inverse matrix!
	for (UINT i = 0 ; i < 3 ; ++i)
	{
		r[i] = Util::ScalarProduct3D(cofactorMatrix.r[i], 1/determinant);
	}
	return true;
}

EnMatrix2x2 EnMatrix3x3::CreateMinor(const int& row, const int& column)
{
	std::vector<float> tempVector;
	for ( UINT i = 0 ; i < 3 ; ++i)
	{
		if (i != row)
		{
			for ( UINT j = 0 ; j < 3 ; ++j)
			{
				if (j != column)
				{
					tempVector.push_back(r[i][j]);
				}
			}
		}
	}
	return EnMatrix2x2(	EnVector2(tempVector[0], tempVector[1]),
						EnVector2(tempVector[2], tempVector[3]));
}

float EnMatrix3x3::GetDeterminant()
{
	return	(r[0].x * r[1].y * r[2].z) +	//AEI
			(r[0].y * r[1].z * r[2].x) +	//BFG
			(r[0].z * r[1].x * r[2].y) -	//CDH
			(r[0].x * r[1].z * r[2].y) -	//AFH
			(r[0].y * r[1].x * r[2].z) -	//BFI
			(r[0].z * r[1].y * r[2].x);		//CEG
}

// Utilities that I might find useful
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

	EnVector3 ScalarDivision3D(const EnVector3& v, const float& s)
	{
		float x = v.x / s;
		float y = v.y / s;
		float z = v.z / s;
		return EnVector3(x, y, z);
	}

	EnMatrix3x3 ScalarProduct3x3(const EnMatrix3x3& m, const float& s)
	{
		EnMatrix3x3 newMatrix;
		for(int i = 0 ; i < 3 ; ++i)
		{
			newMatrix.r[i].x = m.r[i].x * s;
			newMatrix.r[i].y = m.r[i].y * s;
			newMatrix.r[i].z = m.r[i].z * s;
		}
		return newMatrix;
	}

	template<class T> void SwapValues(T& a, T& b)
	{
		T c = a;
		a = b;
		b = c;
	}
}

