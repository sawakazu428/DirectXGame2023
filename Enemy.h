#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "TextureManager.h"
#include <cassert>

class Enemy
{
public:
	void Initialize(Model* model, const Vector3& pos);

	void Update();

	void Draw(const ViewProjection& view);

private:
    // ワールドトランスフォーム
	WorldTransform worldTransformEnemy_;
    // 3Dモデル
    Model* modelEnemy_;
    // テクスチャハンドル
    uint32_t textureHandleEnemy_;

	// 速度
	Vector3 velocityEnemy_;
};
