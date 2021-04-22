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

bool GraphicsMath::Inverse(const Matrix& matrix, Matrix& inverseMatrix)
{
	constexpr int N = 4;
	const float det = GetDet(matrix, N);
	if (exmath::compare(det, 0))
		return false;
	const auto inv = &inverseMatrix;
	const auto adj = new Matrix();
	GetAdj(matrix, *adj);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			inv[0][i * 4 + j] = adj[0][i * 4 + j] / det;
	delete adj;
	return true;
}
void GraphicsMath::GetCFactor(const Matrix& in, Matrix& out, int p, int q, int n)
{
	const Matrix* M = &in;
	const auto t = &out;
	int i = 0, j = 0;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) //Copy only those elements which are not in given row r and column c: {
			if (r != p && c != q) {
				t[0][i * 4 + j++] = M[0][r * 4 + c]; //If row is filled increase r index and reset c index
				if (j == n - 1) {
					j = 0; i++;
				}
			}
	}
}
float GraphicsMath::GetDet(const Matrix& matrix, int n)
{
	const auto M = &matrix;
	Matrix tempMatrix;
	const auto t = &tempMatrix;
	float D = 0;
	if (n == 1)
		return M[0][0];
	float s = 1;
	for (int f = 0; f < n; f++) {
		GetCFactor(matrix, tempMatrix, 0, f, n);
		D += s * M[0][f] * GetDet(tempMatrix, n - 1);
		s = -s;
	}
	return D;
}

void GraphicsMath::GetAdj(const Matrix& in, Matrix& out)
{
	const auto adj = &out;
	Matrix tempMatrix;
	const auto t = &tempMatrix;
	float s = 1;
	constexpr int N = 4;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			GetCFactor(in, tempMatrix, i, j, N);
			s = ((i + j) % 2 == 0) ? 1 : -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
			adj[0][j * 4 + i] = s * (GetDet(tempMatrix, N - 1)); //Interchange rows and columns to get the transpose of the cofactor matrix
		}
	}
}

void GraphicsMath::Modulo(Vector3& out, const float& f)
{
	out.x = exmath::modulo(out.x, f);
	out.y = exmath::modulo(out.y, f);
	out.z = exmath::modulo(out.z, f);
}
