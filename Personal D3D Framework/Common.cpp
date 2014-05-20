#include "Common.h"

//======== Quaternion (For Rotations)
// A Quaternion is made up of four parts, 1 scalar and a 3D vector.
// The vector is often written as (i, j, k) to denote the imaginary numbers that it uses.
// However, to get the usefulness of the already present 3D vectors, I'll call them (x, y, z).
// That is, unless someone can think of a reason not to...


Quaternion::Quaternion()
	: scalar(1), vector(EnVector3::Zero()) {}

Quaternion::Quaternion(const float& initS, const EnVector3& initVector)
	: scalar(initS), vector(initVector) {}

Quaternion::Quaternion(const float& initS, const float& initX, const float& initY, const float& initZ)
	: scalar(initS), vector(initX, initY, initZ) {}

Quaternion::~Quaternion() {}

Quaternion Quaternion::Normalized()
{
	float d = (scalar*scalar) + vector.GetMagnitude();
	if (d == 0)
	{
		return Quaternion();
	}
	return Quaternion((scalar/d), (vector.x/d), (vector.y/d), (vector.z/d));
}

//Member operators
Quaternion& Quaternion::operator*= (const Quaternion& rhs)
{
	scalar =  scalar * rhs.scalar - vector.ADot(rhs.vector);
	vector = EnVector3(	Util::ScalarProduct3D(rhs.vector, scalar) +
						Util::ScalarProduct3D(vector, rhs.scalar) +
						vector.Cross(rhs.vector));
	return *this;
}


//Non-Member operators
Quaternion operator* (Quaternion lhs, const Quaternion& rhs)
{
	lhs *= rhs;
	return lhs;
}

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
	return EnVector3((rhs.c[0].x * x) + (rhs.c[0].y * y) + (rhs.c[0].z * z),
					(rhs.c[1].x * x) + (rhs.c[1].y * y) + (rhs.c[1].z * z),
					(rhs.c[2].x * x) + (rhs.c[2].y * y) + (rhs.c[2].z * z));
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


//======== Engine Vector4 (Aka, 4 dimensional vector)
EnVector4::EnVector4()
	: x(0), y(0), z(0), w(0) {}

EnVector4::EnVector4(const float& initX, const float& initY, const float& initZ, const float& initW)
{
	x = initX;
	y = initY;
	z = initZ;
	w = initW;
}

EnVector4::~EnVector4()
{
}

EnVector4 EnVector4::Zero()
{
	return EnVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

EnVector4 EnVector4::Normalized()
{
	float mag = GetMagnitude();
	if (mag == 0) { return EnVector4(); }
	return EnVector4((x/mag), (y/mag), (z/mag), (w/mag));
}

float EnVector4::GetMagnitude() const
{
	float a = x * x;
	float b = y * y;
	float c = z * z;
	float d = w * w;
	return sqrt(a+b+c+d);
}

//======== Engine Matrix2x2
//	In the Form:
//	| A B |
//	| C D |
//	Where each column is a 2D Vector. Eg (A,C) & (B,D)
EnMatrix2x2::EnMatrix2x2()
{
	//Init into Identity Matrix;
	c[0] = EnVector2(1.0f, 0.0f);
	c[1] = EnVector2(0.0f, 1.0f);
}

EnMatrix2x2::EnMatrix2x2(const EnVector2& c1, const EnVector2& c2)
{
	c[0] = c1;
	c[1] = c2;
}

EnMatrix2x2::~EnMatrix2x2()
{
}

EnMatrix2x2 EnMatrix2x2::Identity()
{
	return EnMatrix2x2(	EnVector2(1.0f, 0.0f),
						EnVector2(0.0f, 1.0f));
}

float EnMatrix2x2::GetDeterminant()
{
	return	(c[0].x * c[1].y) -
			(c[0].y * c[1].x);
}

//	Engine Matrix3x3
//	In the Form:
//	| A B C |
//	| D E F |
//	| G H I |
//	Where each column is a 3D Vector. Eg (A,D,G), (B,E,H), (C,F,I)
EnMatrix3x3::EnMatrix3x3()
{
	//Init into Identity Matrix;
	c[0] = EnVector3(1.0f, 0.0f, 0.0f);
	c[1] = EnVector3(0.0f, 1.0f, 0.0f);
	c[2] = EnVector3(0.0f, 0.0f, 1.0f);
}

EnMatrix3x3::EnMatrix3x3(const EnVector3& c1, const EnVector3& c2, const EnVector3& c3)
{
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
}

EnMatrix3x3::~EnMatrix3x3()
{
}

EnMatrix3x3 EnMatrix3x3::Identity()
{
	return EnMatrix3x3(	EnVector3(1.0f, 0.0f, 0.0f),
						EnVector3(0.0f, 1.0f, 0.0f),
						EnVector3(0.0f, 0.0f, 1.0f));
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
				cofactorMatrix.c[i][j] = pow(-1, (i + j + 2)) * minorMatrix.GetDeterminant();
			}
		}
		//Now to flip the Matrix along the left->right diagonal.
		cofactorMatrix.Transpose();
	};
	//Finally, multiply the cofactor matrix by the reciprocal of the determinant
	//to get the inverse matrix!
	for (UINT i = 0 ; i < 3 ; ++i)
	{
		c[i] = Util::ScalarProduct3D(cofactorMatrix.c[i], 1/determinant);
	}
	return true;
}

void EnMatrix3x3::Transpose()
{
	Util::SwapValues(c[1][0], c[0][1]);
	Util::SwapValues(c[2][0], c[0][2]);
	Util::SwapValues(c[2][1], c[1][2]);
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
					tempVector.push_back(c[i][j]);
				}
			}
		}
	}
	return EnMatrix2x2(	EnVector2(tempVector[0], tempVector[1]),
						EnVector2(tempVector[2], tempVector[3]));
}

float EnMatrix3x3::GetDeterminant()
{
	return	(c[0].x * c[1].y * c[2].z) +	//AEI
			(c[0].y * c[1].z * c[2].x) +	//BFG
			(c[0].z * c[1].x * c[2].y) -	//CDH
			(c[0].x * c[1].z * c[2].y) -	//AFH
			(c[0].y * c[1].x * c[2].z) -	//BFI
			(c[0].z * c[1].y * c[2].x);		//CEG
}

//======== Engine Matrix4x4
//	In the Form:
//	| A B C D |
//	| E F G H |
//	| I J K L |
//	| M N O P |
//	Where each column is a 4D vector. Eg (A,E,I,M), (B,F,J,N)... etc
EnMatrix4x4::EnMatrix4x4()
{
	//Init into Identity Matrix;
	c[0] = EnVector4(1.0f, 0.0f, 0.0f, 0.0f);
	c[1] = EnVector4(0.0f, 1.0f, 0.0f, 0.0f);
	c[2] = EnVector4(0.0f, 0.0f, 1.0f, 0.0f);
	c[3] = EnVector4(0.0f, 0.0f, 0.0f, 1.0f);
	
}

EnMatrix4x4::EnMatrix4x4(const EnVector4& c1, const EnVector4& c2, const EnVector4& c3, const EnVector4& c4)
{
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	c[3] = c4;
}

EnMatrix4x4::~EnMatrix4x4()
{
}

EnMatrix4x4 EnMatrix4x4::Identity()
{
	return EnMatrix4x4(	EnVector4(1.0f, 0.0f, 0.0f, 0.0f),
						EnVector4(0.0f, 1.0f, 0.0f, 0.0f),
						EnVector4(0.0f, 0.0f, 1.0f, 0.0f),
						EnVector4(0.0f, 0.0f, 0.0f, 1.0f));
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
			newMatrix.c[i].x = m.c[i].x * s;
			newMatrix.c[i].y = m.c[i].y * s;
			newMatrix.c[i].z = m.c[i].z * s;
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

