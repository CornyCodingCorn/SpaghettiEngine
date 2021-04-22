#include "GraphicsMath.h"

float GraphicsMath::ToRad(const float& degree)
{
	return degree * (pi / 180.0f);
}

bool GraphicsMath::CompareFloat(float x, float y, float epsilon)
{
	if (fabs(x - y) < epsilon)
		return true;
	return false;
}

void GraphicsMath::TransformVector3(Vector3* outVec, const Vector3* inVec, PMatrix transformMatrix)
{
	Vector4 tempValue;
	D3DXVec3Transform(&tempValue, inVec, transformMatrix);
	outVec->x = tempValue.x;
	outVec->y = tempValue.y;
	outVec->z = tempValue.z;
}

PMatrix GraphicsMath::NewMatrix()
{
	return new Matrix(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	);
}

void GraphicsMath::ZeroMatrix(Matrix* outMatrix)
{
	for (int i = 0; i < 4; i++)
	{
		outMatrix->_11 = 0;
		outMatrix->_21 = 0;
		outMatrix->_31 = 0;
		outMatrix->_41 = 0;
		outMatrix->_12 = 0;
		outMatrix->_22 = 0;
		outMatrix->_32 = 0;
		outMatrix->_42 = 0;
		outMatrix->_13 = 0;
		outMatrix->_23 = 0;
		outMatrix->_33 = 0;
		outMatrix->_43 = 0;
		outMatrix->_14 = 0;
		outMatrix->_24 = 0;
		outMatrix->_34 = 0;
		outMatrix->_44 = 0;
	}
}

void GraphicsMath::RoundVector3(Vector3* vec)
{
	vec->x = std::round(vec->x);
	vec->y = std::round(vec->y);
	vec->z = std::round(vec->z);
}

void GraphicsMath::TranslateMatrix(Matrix& matrix, const Vector3& vec)
{
	matrix._41 += vec.x;
	matrix._42 += vec.y;
	matrix._43 += vec.z;
}

void GraphicsMath::MoveMatrix(Matrix& matrix, const Vector3& vec)
{
	matrix._41 = vec.x;
	matrix._42 = vec.y;
	matrix._43 = vec.z;
}

void GraphicsMath::TranslateMatrix(Matrix& matrix, const float& x, const float& y, const float& z)
{
	matrix._41 += x;
	matrix._42 += y;
	matrix._43 += z;
}

void GraphicsMath::MoveMatrix(Matrix& matrix, const float& x, const float& y, const float& z)
{
	matrix._41 = x;
	matrix._42 = y;
	matrix._43 = z;
}

void GraphicsMath::Modulo(Vector3& out, const float& f)
{
	out.x = exmath::modulo(out.x, f);
	out.y = exmath::modulo(out.y, f);
	out.z = exmath::modulo(out.z, f);
}
