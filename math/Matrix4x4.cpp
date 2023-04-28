#include "Matrix4x4.h"

// スケーリング行列の作成	
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result;

	result = {
		scale.x,0.0f,0.0f,0.0f,
		0.0f,scale.y,0.0f,0.0f,
		0.0f,0.0f,scale.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
};

// 回転行列の作成？
Matrix4x4 MakeRotateXMatrix(float theta) {

	float sin = std::sinf(theta);	
	float cos = std::cosf(theta);

	Matrix4x4 result;
	result = {1.0f, 0.0f, 0.0f,0.0f,
		      0.0f, cos, sin, 0.0f,
	          0.0f,    -sin, cos, 0.0f, 
		      0.0f, 0.0f,    0.0f, 1.0f
	};


	return result;
}

Matrix4x4 MakeRotateYMatrix(float theta) {

	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;
	result = {cos, 0.0f, -sin, 0.0f,
		      0.0f,1.0f,  0.0f,  0.0f,
	          sin, 0.0f, cos,  0.0f,
		      0.0f, 0.0f, 0.0f, 1.0f};

	return result;
}

Matrix4x4 MakeRotateZMatrix(float theta) {

	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result;
	result = {cos, sin, 0.0f,0.0f,
		      -sin, cos,0.0f,0.0f,
	          0.0f, 0.0f,1.0f,0.0f, 
		      0.0f, 0.0f,0.0f,1.0f};

	return result;
}
// 平行移動行列の作成

Matrix4x4 MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 result;

	result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate.x,translate.y,translate.z,1.0f
	};
	return result;
}

Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;

	return result *= m2;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate)
{
	// スケーリング行列
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	// 回転行列
	Matrix4x4 matRotX = MakeRotateXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotateXMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotateXMatrix(rot.z);

	// 合成(X*Y*Z)
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// 平行移動行列
	Matrix4x4 matTrans = MakeTranslateMatrix(translate);

	// 合成
	Matrix4x4 result = matScale * matRot * matTrans;
	
};
