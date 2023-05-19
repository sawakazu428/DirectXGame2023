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

	void ApproachUpdate(const float kEnemySpeed);

	void LeaveUpdate(const float kEnemySpeed);

private:
    // ワールドトランスフォーム
	WorldTransform worldTransformEnemy_;
    // 3Dモデル
    Model* modelEnemy_;
    // テクスチャハンドル
    uint32_t textureHandleEnemy_;

	// 速度
	Vector3 velocityEnemy_;

// 行動フェーズ
enum class Phase 
{
	Approach, // 接近する
	Leave,    // 離脱する
};
	Phase phase_ = Phase::Approach;
};

