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

Matrix4x4 Inverse(const Matrix4x4& m);

// 代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

// 2公演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);


float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);




