#pragma once
#include "Vector3.h"
#define USE_MATH_DEFINES
#include"cmath"
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};
// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
// 回転行列の作成？
Matrix4x4 MakeRotateXMatrix(float theta);
// 回転行列の作成？
Matrix4x4 MakeRotateYMatrix(float theta);
// 回転行列の作成？
Matrix4x4 MakeRotateZMatrix(float theta);

// 平行移動行列の作成
Matrix4x4 MakeTranslateMatrix(Vector3 translate);

// スケーリング行列の作成
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

// 2公演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float determinant = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	                    m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	                    m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
	                    m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	                    m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	                    m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
	                    m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	                    m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	                    m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
	                    m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	                    m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	                    m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	                    m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	                    m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	                    m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
	                    m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	                    m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	                    m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
	                    m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	                    m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	                    m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
	                    m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	                    m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	                    m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	assert(determinant != 0.0f);
	float determinantRecp = 1.0f / determinant;

	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
	                  m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	                  m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) *
	                 determinantRecp;
	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
	                  m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
	                  m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) *
	                 determinantRecp;
	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
	                  m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	                  m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) *
	                 determinantRecp;
	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
	                  m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
	                  m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) *
	                 determinantRecp;
	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
	                  m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
	                  m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) *
	                 determinantRecp;
	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
	                  m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	                  m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) *
	                 determinantRecp;
	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
	                  m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
	                  m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) *
	                 determinantRecp;

	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
	                  m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	                  m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) *
	                 determinantRecp;

	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
	                  m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	                  m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][1]) *
	                 determinantRecp;
	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
	                  m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
	                  m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) *
	                 determinantRecp;
	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
	                  m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	                  m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) *
	                 determinantRecp;
	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
	                  m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
	                  m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) *
	                 determinantRecp;
	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
	                  m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
	                  m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) *
	                 determinantRecp;
	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
	                  m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	                  m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) *
	                 determinantRecp;
	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
	                  m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
	                  m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) *
	                 determinantRecp;
	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
	                  m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	                  m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) *
	                 determinantRecp;
	return result;
};



