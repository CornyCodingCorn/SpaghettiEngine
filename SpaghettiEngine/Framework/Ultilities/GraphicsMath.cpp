#include "GraphicsMath.h"

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
	vec->x = static_cast<int>(vec->x + 0.5f);
	vec->y = static_cast<int>(vec->y + 0.5f);
	vec->z = static_cast<int>(vec->z + 0.5f);
}